#include "mainwindow.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //
    int fontId = QFontDatabase::addApplicationFont(
        ":/fonts/Fonts/Kreon/Kreon-VariableFont_wght.ttf");
    if (fontId != -1) {
        const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty())
            a.setFont(QFont(families.first()));
    }

    QString globalStyle = R"(
        QCheckBox::indicator {
            width: 24px;
            height: 24px;
        }
        QCheckBox::indicator:unchecked {
            image: url(:/icons/Pics/Icons/checkbox_unticked.png);
        }
        QCheckBox::indicator:checked {
            image: url(:/icons/Pics/Icons/checkbox_ticked.png);
        }
    )";
    //

    a.setStyleSheet(globalStyle);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
