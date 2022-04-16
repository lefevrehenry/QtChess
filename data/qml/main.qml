import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml 2.14

import Toto 1.0

ColumnLayout {
    id: root

    anchors.fill: parent

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 64

        color: "green"

        Label {
            anchors.centerIn: parent
            text: "32"
        }
    }

    Button {
        text: "ok"
        onClicked: {
            console.log("quit")
            Qt.quit();
        }
    }

    A {
        id: a
    }
}
