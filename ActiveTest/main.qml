import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow {
    visible: true
    width: 300
    height: 150
    title: "Hello ForwardT"

//    Timer {
//        id: updateTimer
//        running: true
//        repeat: true
//        interval: 1 * 1000
//        onTriggered: {
//            if (smsObjectManager.isObjectAlive()) {
//                console.log("Object is alive");
//            } else {
//                console.log("Object is not alive");
//            }
//        }
//    }

    ColumnLayout {
        TextField {
            id: text
            placeholderText: "text"
        }

        TextField {
            id: nick
            placeholderText: "nick"
        }

        Button {
            text: qsTr("send message")
            onClicked: {
                var callback = function () {
                    console.log("message sended");
                }

                //smsObjectManager.sendMessage(text.text, nick.text, callback);
            }
        }
    }
}
