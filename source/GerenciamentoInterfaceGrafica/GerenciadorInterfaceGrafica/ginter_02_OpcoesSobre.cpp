#include "ginter_02_OpcoesSobre.hpp"

#include "source/utils/ApplicationPathUtils.hpp"

QString SITE_LICENSES() {
    return QStringLiteral(R"(
    https://www.gnu.org/licenses/lgpl-3.0.html.en
    https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.en
    https://opensource.org/licenses/BSD-2-Clause
    https://opensource.org/licenses/BSD-3-Clause
    https://www.apache.org/licenses/LICENSE-2.0
    https://python-pillow.org/
    https://opensource.org/licenses/MIT
    https://opensource.org/license/isc-license-txt
    )");
}

QString LICENSE_TEXT_PT_BR() {
    static const QString v = load_text_file("EULA_pt_BR.txt", "EULA");
    return v;
}

QString LICENSE_TEXT_EN_US() {
    static const QString v = load_text_file("EULA_en_US.txt", "EULA");
    return v;
}

QString NOTICE_TEXT_PT_BR() {
    static const QString v = load_text_file("NOTICE_pt_BR.txt", "NOTICES");
    return v;
}

QString NOTICE_TEXT_EN_US() {
    static const QString v = load_text_file("NOTICE_en_US.txt", "NOTICES");
    return v;
}

QString ABOUT_TEXT_PT_BR() {
    static const QString v = load_text_file("ABOUT_pt_BR.txt", "ABOUT");
    return v;
}

QString ABOUT_TEXT_EN_US() {
    static const QString v = load_text_file("ABOUT_en_US.txt", "ABOUT");
    return v;
}

QString Privacy_Policy_pt_BR() {
    static const QString v = load_text_file("Privacy_Policy_pt_BR.txt", "PRIVACY_POLICY");
    return v;
}

QString Privacy_Policy_en_US() {
    static const QString v = load_text_file("Privacy_Policy_en_US.txt", "PRIVACY_POLICY");
    return v;
}

QString History_APP_pt_BR() {
    static const QString v = load_text_file("History_APP_pt_BR.txt", "ABOUT");
    return v;
}

QString History_APP_en_US() {
    static const QString v = load_text_file("History_APP_en_US.txt", "ABOUT");
    return v;
}

QString RELEASE_NOTES_pt_BR() {
    static const QString v = load_text_file("RELEASE NOTES_pt_BR.txt", "RELEASE");
    return v;
}

QString RELEASE_NOTES_en_US() {
    static const QString v = load_text_file("RELEASE NOTES_en_US.txt", "RELEASE");
    return v;
}
