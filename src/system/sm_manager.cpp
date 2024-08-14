/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sm_manager.h"

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include "index/ix.h"
#include "record/rm.h"
#include "record_printer.h"

/**
 * @description: 判断是否为一个文件夹
 * @return {bool} 返回是否为一个文件夹
 * @param {string&} db_name 数据库文件名称，与文件夹同名
 */
bool SmManager::is_dir(const std::string& db_name) {
    struct stat st;
    return stat(db_name.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

/**
 * @description: 创建数据库，所有的数据库相关文件都放在数据库同名文件夹下
 * @param {string&} db_name 数据库名称
 */
void SmManager::create_db(const std::string& db_name) {
    if (is_dir(db_name)) {
        throw DatabaseExistsError(db_name);
    }
    //为数据库创建一个子目录
    std::string cmd = "mkdir " + db_name;
    if (system(cmd.c_str()) < 0) {  // 创建一个名为db_name的目录
        throw UnixError();
    }
    if (chdir(db_name.c_str()) < 0) {  // 进入名为db_name的目录
        throw UnixError();
    }
    //创建系统目录
    DbMeta *new_db = new DbMeta();
    new_db->name_ = db_name;

    // 注意，此处ofstream会在当前目录创建(如果没有此文件先创建)和打开一个名为DB_META_NAME的文件
    std::ofstream ofs(DB_META_NAME);

    // 将new_db中的信息，按照定义好的operator<<操作符，写入到ofs打开的DB_META_NAME文件中
    ofs << *new_db;  // 注意：此处重载了操作符<<

    delete new_db;

    // 创建日志文件
    disk_manager_->create_file(LOG_FILE_NAME);
    disk_manager_->create_file(RESTART_FILE_NAME);

    // 回到根目录
    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 删除数据库，同时需要清空相关文件以及数据库同名文件夹
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::drop_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    std::string cmd = "rm -r " + db_name;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
}

/**
 * @description: 打开数据库，找到数据库对应的文件夹，并加载数据库元数据和相关文件
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::open_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    if (chdir(db_name.c_str()) < 0) {  // 进入名为db_name的目录
        throw UnixError();
    }
    std::ifstream ifs(DB_META_NAME);
    ifs >> db_;
    // 打开所有表的文件和索引文件
    for (auto &entry : db_.tabs_) {
        const std::string &tab_name = entry.first;
        fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
        for(auto &ix: entry.second.indexes) {
            ihs_.emplace(ix_manager_->get_index_name(tab_name, ix.cols), ix_manager_->open_index(tab_name, ix.cols));
        }
    }
}

/**
 * @description: 把数据库相关的元数据刷入磁盘中
 */
void SmManager::flush_meta() {
    // 默认清空文件
    std::ofstream ofs(DB_META_NAME);
    ofs << db_;
}

/**
 * @description: 关闭数据库并把数据落盘
 */
void SmManager::close_db() {
    // TODO(zzx): 先这样写，后续再补充
    flush_meta();
    // 将表头写入磁盘
    for (auto &entry : fhs_) {
        auto &fh = entry.second;
        rm_manager_->close_file(fh.get());
    }
    fhs_.clear();
    for(auto &entry : ihs_) {
        auto &ih = entry.second;
        ix_manager_->close_index(ih.get());
    }
    ihs_.clear();
    db_.name_.clear();
    if(chdir("..") < 0) {
        throw UnixError();
    }
    // buffer_pool_manager_->flush_all_pages();
}

/**
 * @description: 显示所有的表,通过测试需要将其结果写入到output.txt,详情看题目文档
 * @param {Context*} context 
 */
void SmManager::show_tables(Context* context) {
    std::fstream outfile;
    outfile.open("output.txt", std::ios::out | std::ios::app);
    outfile << "| Tables |\n";
    RecordPrinter printer(1);
    printer.print_separator(context);
    printer.print_record({"Tables"}, context);
    printer.print_separator(context);
    for (auto &entry : db_.tabs_) {
        auto &tab = entry.second;
        printer.print_record({tab.name}, context);
        outfile << "| " << tab.name << " |\n";
    }
    printer.print_separator(context);
    outfile.close();
}

/**
 * @description: 显示表的元数据
 * @param {string&} tab_name 表名称
 * @param {Context*} context 
 */
void SmManager::desc_table(const std::string& tab_name, Context* context) {
    TabMeta &tab = db_.get_table(tab_name);

    std::vector<std::string> captions = {"Field", "Type", "Index"};
    RecordPrinter printer(captions.size());
    // Print header
    printer.print_separator(context);
    printer.print_record(captions, context);
    printer.print_separator(context);
    // Print fields
    for (auto &col : tab.cols) {
        std::vector<std::string> field_info = {col.name, coltype2str(col.type), col.index ? "YES" : "NO"};
        printer.print_record(field_info, context);
    }
    // Print footer
    printer.print_separator(context);
}

/**
 * @description: 创建表
 * @param {string&} tab_name 表的名称
 * @param {vector<ColDef>&} col_defs 表的字段
 * @param {Context*} context 
 */
void SmManager::create_table(const std::string& tab_name, const std::vector<ColDef>& col_defs, Context* context) {
    if (db_.is_table(tab_name)) {
        throw TableExistsError(tab_name);
    }
    // Create table meta
    int curr_offset = 0;
    TabMeta tab;
    tab.name = tab_name;
    for (auto &col_def : col_defs) {
        ColMeta col = {.tab_name = tab_name,
                       .name = col_def.name,
                       .alias = "",
                       .type = col_def.type,
                       .len = col_def.len,
                       .offset = curr_offset,
                       .index = false};
        curr_offset += col_def.len;
        tab.cols.push_back(col);
    }
    // Create & open record file
    int record_size = curr_offset;  // record_size就是col meta所占的大小（表的元数据也是以记录的形式进行存储的）
    rm_manager_->create_file(tab_name, record_size);
    db_.tabs_[tab_name] = tab;
    // fhs_[tab_name] = rm_manager_->open_file(tab_name);
    fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));

    flush_meta();
}

/**
 * @description: 删除表
 * @param {string&} tab_name 表的名称
 * @param {Context*} context
 */
void SmManager::drop_table(const std::string& tab_name, Context* context) {
    if(!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }
    auto &fh = fhs_[tab_name];
    // 删除所有索引
    auto indexes = db_.get_table(tab_name).indexes;
    for(auto &index: indexes) {
        drop_index(tab_name, index.cols, context);
    }
    // 删除表
    fh->clear_pages();
    rm_manager_->close_file(fh.get());  // 关闭文件
    rm_manager_->destroy_file(tab_name);
    db_.tabs_.erase(tab_name);
    fhs_.erase(tab_name);
    flush_meta();
}

void SmManager::load_table(const std::string& file_name, const std::string& tab_name)
{
    std::ifstream csv_data(file_name, std::ios::in);
    auto fh = fhs_[tab_name].get();
    TabMeta &tab = db_.get_table(tab_name);
    if (!csv_data.is_open())
    {
        throw InternalError("error opening file");
    }
    std::string line;
    std::string word;
    getline(csv_data, line);
    std::istringstream sin;
    RmRecord rec(fh->get_file_hdr().record_size);
    char* key = new char[tab.get_col_total_len()];
    auto& indexes = tab.indexes;
    std::vector<IxIndexHandle*> ihs;
    for(auto& index: indexes) {
        ihs.push_back(ihs_.at(ix_manager_->get_index_name(tab_name, index.cols)).get());
    }
    // 读取每一行数据
    while (getline(csv_data, line))
    {
        sin.clear();
        sin.str(line);
        int curr = 0;
        while (getline(sin, word, ','))
        {
            // Value& curr_value = values[curr];
            Value curr_value;
            curr_value.set_str(word);
            curr_value.value_cast(tab.cols[curr].type);
            auto &col = tab.cols[curr];
            curr_value.init_raw(col.len);
            memcpy(rec.data + col.offset, curr_value.raw->data, col.len);
            curr++;
        }
        Rid rid = fh->insert_record(rec.data, nullptr);
        // 插入索引
        for(size_t i = 0; i < indexes.size(); ++i) {
            auto& index = indexes[i];
            auto& ih = ihs[i];
            int offset = 0;
            for(size_t j = 0; j < (size_t)index.col_num; ++j) {
                memcpy(key + offset, rec.data + index.cols[j].offset, index.cols[j].len);
                offset += index.cols[j].len;
            }
            bool success;
            ih->insert_entry(key, rid, nullptr, &success);
        }
    }
    csv_data.close();
    delete key;
}

// std::queue<std::unique_ptr<RmRecord>> records;
// std::mutex mutexLock{};             
// std::condition_variable condition{};
// bool file_not_end = true;

// struct thread_arg_t {
//     TabMeta* tab;
//     std::string file_name;
//     int record_size;
// };

// void* read_csv(void* arg) {
//     thread_arg_t* thread_arg = (thread_arg_t*)arg;
//     TabMeta* tab = thread_arg->tab;
//     std::string& file_name = thread_arg->file_name;
//     std::ifstream csv_data(file_name, std::ios::in);
//     if (!csv_data.is_open())
//     {
//         throw InternalError("error opening file");
//     }
//     std::string line;
//     std::string word;
//     std::istringstream sin;
//     getline(csv_data, line); // skip the first line
//     while (getline(csv_data, line))
//     {
//         auto rec = std::make_unique<RmRecord>(thread_arg->record_size);
//         sin.clear();
//         sin.str(line);
//         int curr = 0;
//         while (getline(sin, word, ','))
//         {
//             // Value& curr_value = values[curr];
//             Value curr_value;
//             curr_value.set_str(word);
//             curr_value.value_cast(tab->cols[curr].type);
//             auto &col = tab->cols[curr];
//             curr_value.init_raw(col.len);
//             memcpy(rec->data + col.offset, curr_value.raw->data, col.len);
//             curr++;
//         }
//         std::unique_lock<std::mutex> lockGuard(mutexLock);
//         records.push(std::move(rec));
//         lockGuard.unlock();
//         condition.notify_all();
//     }
//     csv_data.close();
//     std::unique_lock<std::mutex> lockGuard(mutexLock);
//     file_not_end = false;
//     lockGuard.unlock();
//     condition.notify_all();
//     return nullptr;
// }

// void SmManager::load_table(const std::string& file_name, const std::string& tab_name)
// {
//     file_not_end = true;
//     bool record_not_end = true;
//     auto fh = fhs_[tab_name].get();
//     TabMeta &tab = db_.get_table(tab_name);
//     char* key = new char[tab.get_col_total_len()];
//     auto& indexes = tab.indexes;
//     std::vector<IxIndexHandle*> ihs;
//     for(auto& index: indexes) {
//         ihs.push_back(ihs_.at(ix_manager_->get_index_name(tab_name, index.cols)).get());
//     }
//     pthread_t thread_id;
//     thread_arg_t thread_arg = {&tab, file_name, fh->get_file_hdr().record_size};
//     pthread_create(&thread_id, nullptr, &read_csv, &thread_arg);
//     // 读取每一行数据
//     while (record_not_end)
//     {
//         std::unique_lock<std::mutex> lockGuard(mutexLock);
//         if(records.empty()) {
//             condition.wait(lockGuard);        
//         }
//         auto rec = std::move(records.front());
//         if(rec == nullptr)
//             break;
//         records.pop();
//         if(!file_not_end && records.empty()) {
//             record_not_end = false;
//         }
//         lockGuard.unlock();
//         // 插入数据
//         Rid rid = fh->insert_record(rec->data, nullptr);
//         // 插入索引
//         for(size_t i = 0; i < indexes.size(); ++i) {
//             auto& index = indexes[i];
//             auto& ih = ihs[i];
//             int offset = 0;
//             for(size_t j = 0; j < (size_t)index.col_num; ++j) {
//                 memcpy(key + offset, rec->data + index.cols[j].offset, index.cols[j].len);
//                 offset += index.cols[j].len;
//             }
//             bool success;
//             ih->insert_entry(key, rid, nullptr, &success);
//         }
//         // delete rec;
//     }
//     pthread_join(thread_id, nullptr);
//     std::cout << "record size: " << records.size() << std::endl;
//     delete key;
// }

/**
 * @description: 创建索引
 * @param {string&} tab_name 表的名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::create_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    std::vector<ColMeta> cols(col_names.size());
    std::vector<int> col_idx(col_names.size());
    TabMeta &tab = db_.get_table(tab_name);
    int col_tot_len = 0;
    for (size_t i = 0; i < col_names.size(); ++i) {
        auto col = tab.get_col(col_names[i]);
        cols[i] = *col;
        col_tot_len += col->len;
        col_idx[i] = col->offset;
    }
    ix_manager_->create_index(tab_name, cols);
    tab.indexes.push_back({tab_name, col_tot_len,(int)cols.size(), cols});
    ihs_.emplace(ix_manager_->get_index_name(tab_name, col_names), std::move(ix_manager_->open_index(tab_name, col_names)));
    flush_meta();
    // 将原有的数据插入索引中
    auto fh = fhs_[tab_name].get();
    auto scan = std::make_unique<RmScan>(fh);
    char* key = new char[tab.get_col_total_len()];
    auto ih = ihs_.at(ix_manager_->get_index_name(tab_name, cols)).get();
    while (!scan->is_end()) {
        auto rec = fh->get_record(scan->rid(), context);
        int offset = 0;
        for(size_t i = 0; i < col_names.size(); ++i) {
            memcpy(key + offset, rec->data + col_idx[i], cols[i].len);
            offset += cols[i].len;
        }
        bool success;
        Transaction* txn = context == nullptr ? nullptr : context->txn_;
        ih->insert_entry(key, scan->rid(), txn, &success);
        if(!success) {
            fh->delete_record(scan->rid(), context);
        }
        scan->next();
    }
    // std::cout << "create index, btree order: " << ih->get_btree_order() << std::endl;
    delete key;
}

static void col_meta_to_string(const std::vector<ColMeta>& cols, std::vector<std::string>& str) {
    str.resize(cols.size());
    for(size_t i = 0; i < cols.size(); ++i) {
        str[i] = cols[i].name;
    }
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    std::string index_name = ix_manager_->get_index_name(tab_name, col_names);
    auto index_it = ihs_.find(index_name);
    if(index_it == ihs_.end()) {
        throw IndexNotFoundError(tab_name, col_names);
    }
    TabMeta &tab = db_.get_table(tab_name);
    auto index_meta = tab.get_index_meta(col_names);
    tab.indexes.erase(index_meta);
    ix_manager_->close_index(index_it->second.get());
    bool res = index_it->second->clear_pages();
    assert(res);
    ix_manager_->destroy_index(tab_name, col_names);
    ihs_.erase(index_it);
    flush_meta();
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<ColMeta>&} 索引包含的字段元数据
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<ColMeta>& cols, Context* context) {
    auto index_it = ihs_.find(ix_manager_->get_index_name(tab_name, cols));
    if(index_it == ihs_.end()) {
        std::vector<std::string> col_names;
        col_meta_to_string(cols, col_names);
        throw IndexNotFoundError(tab_name, col_names);
    }
    TabMeta &tab = db_.get_table(tab_name);
    auto index_meta = tab.get_index_meta(cols);
    tab.indexes.erase(index_meta);
    ix_manager_->close_index(index_it->second.get());
    bool res = index_it->second->clear_pages();
    assert(res);
    ix_manager_->destroy_index(tab_name, cols);
    ihs_.erase(index_it);
    flush_meta();
}

void SmManager::show_index(const std::string& tab_name, Context* context) {
    TabMeta &tab = db_.get_table(tab_name);
    if(tab.indexes.empty()) {
        return;
    }
    std::fstream outfile;
    outfile.open("output.txt", std::ios::out | std::ios::app);
    RecordPrinter printer(3);
    printer.print_separator(context);
    for(auto &index: tab.indexes) {
        std::string col_str = "(";
        for(auto &col: index.cols) {
            col_str += col.name + ",";
        }
        col_str.pop_back();
        col_str += ")";
        printer.print_record({tab_name, "unique", col_str}, context);
        outfile << "| " << tab.name << " | unique | " << col_str << " |\n";
        printer.print_separator(context);
    }
    outfile.close();
}