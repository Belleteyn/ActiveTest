import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: root
    visible: true
    width: 500
    height: 200
    title: "SMSClient"

    Layout.minimumHeight: 100
    Layout.minimumWidth: 400

    property bool active: false
    property bool serverActive: false

    property int id: 0
    property int priority: 0
    property string message: ""

    property string error: ""

    onVisibilityChanged: {
        if (root.visibility === 3) {
            root.hide();
        }
    }

    Connections {
        target: boss

        onTitleActive: {
            active = isTitleActive;

            if (!active) {
                error = "Ошибка: неактивен титровальный элемент";
                if(root.visibility === 0) {
                    root.show();
                }
            }
            else {
                if (serverActive)
                    error = "";
            }
        }

        onServerActive: {
            serverActive = isServerActive;

            if (!serverActive) {
                error = "Ошибка при отправке запроса";
                if(root.visibility === 0) {
                    root.show();
                }
            }
            else {
                if (active)
                    error = "";
            }
        }

        onMessageChanged: {
            id = currentId;
            message = currentMessage;
            priority = currentPriority;
        }
    }

    Connections {
        target: systemTray

        onSignalIconActivated: {
             if(root.visibility === 0) {
                 root.show();
             } else {
                 root.hide();
             }
        }
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            id: firstCol

            Layout.fillHeight: true
            Text { text: "Титровальный элемент активен:" }
            Text { text: "Сервер активен:" }
            Text { text: "Текущее сообщение:" }
            Rectangle {
                Layout.fillHeight: true
                color: "transparent"
            }
        }

        ColumnLayout {
            id: secondCol
            width: root.width - firstCol.width
            Layout.fillHeight: true

            Text { text: active.toString() }
            Text { text: serverActive.toString() }

            RowLayout {
                Text { text: "id:" }
                Text { text: id.toString() }
            }

            RowLayout {
                Text { text: "приоритет:" }
                Text { text: priority.toString() }
            }

            Rectangle {
                //width: secondCol.width - 5
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"

                Text {
                    id: messageText
                    text: message
                    wrapMode: Text.Wrap;

                    width: parent.width
                    height: parent.height
                }
            }
        }
    }

    Rectangle {
        height: parent.height
        width: parent.width
        color: "transparent"

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignHCenter

            text: error
            color: "red"
            font.bold: true
            font.pointSize: 18
            wrapMode: Text.Wrap;
        }
    }
}
