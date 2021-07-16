#include "sqlite3_helper.h"

#include "cim/cim.h"
#include "cim/base/Log.h"

#include "cim/base/file/file_util.h"
#include "cim/base/util/string_util.h"

#include "cim/db/config_dao.h"

namespace cim {
    namespace db {
        SqliteHelper* SqliteHelper::getInstance() {
            static SqliteHelper instance;
            return &instance;
        }

        SqliteHelper::SqliteHelper() {

        }

        SqliteHelper::~SqliteHelper() {

        }

        bool SqliteHelper::init() {
            std::string filename = cim::getChatKitConfig().appConfig.app_data_dir + "/" + kDefaultAppDataFilename;

            if (cim::getChatKitConfig().appConfig.app_data_dir.empty()) {
                filename = kDefaultAppDataFilename;

            } else {
                if (!cim::base::FilePathIsExist(cim::getChatKitConfig().appConfig.app_data_dir, true)) {
                    cim::base::CreateDirectory(cim::getChatKitConfig().appConfig.app_data_dir);
                }
            }

            try {
                //std::string tempName = cim::base::UTF16ToUTF8(filename);

                // check db file exist
                if (!cim::base::FilePathIsExist(filename, false)) {
                    db_ = std::make_shared<SQLite::Database>(filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

                } else {
                    db_ = std::make_shared<SQLite::Database>(filename, SQLite::OPEN_READWRITE);
                }

            } catch (const std::exception& e) {
                LogWarn("open sqlite error:{}", e.what());
                return false;
            }

            if (!ConfigDao::getInstance()->createTable()) {
                return false;
            }

            db_is_open_ = true;
            return true;
        }

        void SqliteHelper::cleanup() {
            if (db_is_open_) {
                db_is_open_ = false;

                // auto close
                db_ = nullptr;
            }
        }

        std::shared_ptr<SQLite::Database> SqliteHelper::getConn() const {
            assert(db_ != nullptr);
            return db_;
        }
    }
}

