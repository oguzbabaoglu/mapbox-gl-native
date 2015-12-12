#include <mbgl/storage/frontline_file_source.hpp>
#include <mbgl/storage/response.hpp>

#include <mbgl/map/tile_id.hpp>
#include <mbgl/util/thread.hpp>
#include <mbgl/util/exception.hpp>
#include <mbgl/util/io.hpp>
#include <mbgl/util/mapbox.hpp>
#include <mbgl/util/string.hpp>
#include <mbgl/platform/log.hpp>

#include <cassert>
#include <stdlib.h>

#include "sqlite3.hpp"
#include <sqlite3.h>

namespace mbgl {

using namespace mapbox::sqlite;

FrontlineFileSource::FrontlineFileSource(const std::string& path)
    : thread(std::make_unique<util::Thread<Impl>>(util::ThreadContext{ "FrontlineFileSource", util::ThreadType::Unknown, util::ThreadPriority::Low }, path)) {
}

FrontlineFileSource::~FrontlineFileSource() = default;

class FrontlineFileSource::Impl {
public:
    explicit Impl(const std::string& path);
    ~Impl();

    bool handlesResource(const Resource&);
    void handleRequest(Resource, FileRequest*, Callback);
    void cancelRequest(Resource, FileRequest*);

private:
    void openDatabase();
    std::unique_ptr<Statement> statementForResource(const Resource&, bool checkOnly = false);

private:
    const std::string path;
    std::unique_ptr<::mapbox::sqlite::Database> db;
};

FrontlineFileSource::Impl::Impl(const std::string& path_)
    : path(path_) {
}

FrontlineFileSource::Impl::~Impl() {
    try {
        db.reset();
    } catch (mapbox::sqlite::Exception& ex) {
        Log::Error(Event::Database, ex.code, ex.what());
    }
}

void FrontlineFileSource::Impl::openDatabase() {
    db = std::make_unique<Database>(path.c_str(), ReadOnly);
}

std::unique_ptr<Statement> FrontlineFileSource::Impl::statementForResource(const Resource& resource, bool checkOnly) {
    try {
        if (!db) {
            openDatabase();
        }

        if (resource.kind == Resource::Kind::Tile) {

            const auto canonicalURL = util::mapbox::canonicalURL(resource.url);
            auto parts = util::split(canonicalURL, "/");
            const int8_t  z = atoi(parts[parts.size() - 3].c_str());
            const int32_t x = atoi(parts[parts.size() - 2].c_str());
            const int32_t y = atoi(util::split(util::split(parts[parts.size() - 1], ".")[0], "@")[0].c_str());

            const auto id = TileID(z, x, (pow(2, z) - y - 1), z); // flip y for MBTiles

            const auto sql = (checkOnly ?
                "SELECT COUNT(`tile_data`) FROM `tiles` WHERE `zoom_level` = ? AND `tile_column` = ? AND `tile_row` = ?" :
                "SELECT `tile_data` FROM `tiles` WHERE `zoom_level` = ? AND `tile_column` = ? AND `tile_row` = ?");

            Statement getStmt = db->prepare(sql);

            getStmt.bind(1, (int)id.z);
            getStmt.bind(2, (int)id.x);
            getStmt.bind(3, (int)id.y);

            return std::make_unique<Statement>(std::move(getStmt));

        } else if (resource.kind != Resource::Kind::Unknown) {

            std::string key = "";
            if (resource.kind == Resource::Kind::Glyphs) {
                key = "gl_glyph";
            } else if (resource.kind == Resource::Kind::Source) {
                key = "gl_source";
            } else if (resource.kind == Resource::Kind::SpriteImage) {
                key = "gl_sprite_image";
            } else if (resource.kind == Resource::Kind::SpriteJSON) {
                key = "gl_sprite_metadata";
            } else if (resource.kind == Resource::Kind::Style) {
                key = "gl_style";
            }
            assert(key.length());

            const auto sql = (checkOnly ?
                "SELECT COUNT(`value`) FROM `metadata` WHERE `name` = ?" :
                "SELECT `value` FROM `metadata` WHERE `name` = ?");

            Statement getStmt = db->prepare(sql);

            const auto name = key + "_" + util::mapbox::canonicalURL(resource.url);
            getStmt.bind(1, name.c_str());

            return std::make_unique<Statement>(std::move(getStmt));

        }
    } catch (mapbox::sqlite::Exception& ex) {
        Log::Error(Event::Database, ex.code, ex.what());
    } catch (std::runtime_error& ex) {
        Log::Error(Event::Database, ex.what());
    }

    return nullptr;
}

bool FrontlineFileSource::Impl::handlesResource(const Resource& resource) {
    const auto getStmt = statementForResource(resource, true);

    if (getStmt != nullptr) {
        return (getStmt->run() && getStmt->get<int>(0) > 0);
    }

    return false;
}

void FrontlineFileSource::Impl::handleRequest(Resource resource, FileRequest* req, Callback callback) {
    (void)req;

    std::shared_ptr<Response> res = std::make_shared<Response>();

    const auto getStmt = statementForResource(resource);

    if (getStmt != nullptr) {
        if (getStmt->run()) {
            res->data = std::make_shared<std::string>(std::move(getStmt->get<std::string>(0)));
        } else {
            res->error = std::make_unique<Response::Error>(Response::Error::Reason::NotFound);
        }
    }

    callback(*res);
}

void FrontlineFileSource::Impl::cancelRequest(Resource resource, FileRequest* req) {
    // assume local/offline sources are too fast to be cancellable
    (void)resource;
    (void)req;
}

bool FrontlineFileSource::handlesResource(const Resource& res) {
    return thread->invokeSync<bool>(&Impl::handlesResource, res);
}

std::unique_ptr<FileRequest> FrontlineFileSource::request(const Resource& resource, Callback callback) {
    if (!callback) {
        throw util::MisuseException("FileSource callback can't be empty");
    }

    std::string url;

    switch (resource.kind) {
    case Resource::Kind::Style:
        url = mbgl::util::mapbox::normalizeStyleURL(resource.url, "foo");
        break;

    case Resource::Kind::Source:
        url = util::mapbox::normalizeSourceURL(resource.url, "foo");
        break;

    case Resource::Kind::Glyphs:
        url = util::mapbox::normalizeGlyphsURL(resource.url, "foo");
        break;

    case Resource::Kind::SpriteImage:
    case Resource::Kind::SpriteJSON:
        url = util::mapbox::normalizeSpriteURL(resource.url, "foo");
        break;
        
    default:
        url = resource.url;
    }

    Resource res { resource.kind, url };
    auto req = std::make_unique<FrontlineFileRequest>(res, *this);
    req->workRequest = thread->invokeWithCallback(&Impl::handleRequest, callback, res, req.get());
    return std::move(req);
}

} // namespace mbgl