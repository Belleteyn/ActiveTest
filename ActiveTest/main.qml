import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 150
    title: "Hello ForwardT"

    property bool active: false
    property bool serverActive: false

    property int id: 0
    property int priority: 0
    property string message: ""

    onVisibilityChanged: {
        if (root.visibility === 3) {
            root.hide();
        }
    }

    Connections {
        target: boss

        onTitleActive: {
            active = isTitleActive;
        }

        onServerActive: {
            serverActive = isServerActive;
        }

        onMessageChanged: {
            id = currentId;
            message = currentMessage;
            priority = currentPriority;

            messageText.text = currentMessage;
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

    ColumnLayout {
        id: firstCol
        Text { text: "Титровальный элемент активен:" }
        Text { text: "Сервер активен:" }
        Text { text: "Текущее сообщение:" }
    }

    ColumnLayout {
        id: secondCol
        width: root.width - firstCol.width
        anchors.top:  firstCol.top
        anchors.left: firstCol.right
        anchors.leftMargin: 5

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
        RowLayout {
            Text { text: "текст:" }
            Text {
                id: messageText
                text: ""
                wrapMode: Text.WrapAnywhere;
                width: parent.width
                height: parent.height
            }
        }
    }
}
