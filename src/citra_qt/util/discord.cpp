// Copyright 2019 Citra Valentin Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <QDesktopServices>
#include <QString>
#include <QUrl>
#include <httplib.h>
#include <json.hpp>
#include "citra_qt/uisettings.h"
#include "citra_qt/util/discord.h"
#include "common/assert.h"

namespace DiscordUtil {

std::shared_ptr<httplib::Response> GetToken() {
    httplib::SSLClient api_client(
        QUrl(UISettings::values.cv_web_api_url).host().toStdString().c_str());

    QDesktopServices::openUrl(
        QUrl(QStringLiteral("https://discordapp.com/api/oauth2/"
                            "authorize?client_id=633487273413050418&redirect_uri=http%3A%2F%"
                            "2F127.0.0.1%3A6310&response_type=code&scope=identify")));

    std::shared_ptr<httplib::Response> response;
    httplib::Server server;

    server.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        const std::string code = req.params.find("code")->second;

        response = api_client.Post("/discord/jwt", code, "text/plain");
        if (response == nullptr) {
            LOG_ERROR(Frontend, "Get JWT request failed");
            res.status = 500;
        } else if (response->status != 200) {
            LOG_ERROR(Frontend, "Get JWT request failed, status code: {}, body: {}",
                      response->status, response->body);
            res.status = response->status;
            res.set_content(response->body,
                            httplib::detail::get_header_value(response->headers, "content-type", 0,
                                                              "text/plain"));
        } else {
            res.status = 200;
            res.set_content("You can close this tab", "text/plain");
        }

        server.stop();
    });

    server.listen("127.0.0.1", 6310);

    return response;
}

} // namespace DiscordUtil
