import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: root

    visible: true

    width: 480
    height: 640

    title: qsTr("SMSClient")

    property bool titleActive: false
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
        onAppendLogString: {
            logModel.addMessage(tag, message);
        }

        onTitleActive: {
            titleActive = isTitleActive;

            if (!titleActive) {
                if(root.visibility === 0) {
                    root.show();
                }
            }
        }

        onServerActiveChanged: {
            serverActive = isServerActive;

            if (!serverActive) {
                if(root.visibility === 0) {
                    root.show();
                }
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

    ListModel {
        id: logModel

        property int maxCount: 300

        function addMessage(tag, message) {
            if (count == maxCount) {
                remove(0, 1);
            }
            append({"tag": tag, "message": message});
            logView.positionViewAtEnd();
        }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        SectionHeader {
            Layout.fillWidth: true
            height: 25

            text: qsTr("Status")
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignCenter

            columns: 2
            rows: 2

            columnSpacing: 10

            DesignLabel { text: qsTr("Title") + ":";  Layout.alignment: Qt.AlignRight}
            DesignLabel {
                text: root.titleActive ? qsTr("active") : qsTr("not active")
                color: root.titleActive ? "green" : "red"
                font.bold: true
            }

            DesignLabel { text:qsTr("Server") + ":";  Layout.alignment: Qt.AlignRight }
            DesignLabel {
                text: root.titleActive ? (root.serverActive ? qsTr("active") : qsTr("not active")) : qsTr("unknown")
                color: root.titleActive ? (root.serverActive ? "green" : "red") : "red"
                font.bold: true
            }
        }

        SectionHeader {
            Layout.fillWidth: true
            height: 25

            text: qsTr("Current message")
        }

        GridLayout {

            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignCenter

            columns: 2
            rows: 3

            columnSpacing: 10

            DesignLabel { text: qsTr("id") + ":"; Layout.alignment: Qt.AlignRight }
            DesignLabel { text: root.id }

            DesignLabel { text: qsTr("priority") + ":"; Layout.alignment: Qt.AlignRight }
            DesignLabel { text: root.priority }

            DesignLabel { text: qsTr("message") + ":"; Layout.alignment: Qt.AlignRight }
            DesignLabel { text: root.message }
        }

        SectionHeader {
            Layout.fillWidth: true
            height: 25

            text: qsTr("Log")
        }

        Rectangle {
            Layout.fillWidth: true
            height: 25

            LogDelegate {
                anchors.fill: parent

                time: qsTr("time")
                tag: qsTr("tag")
                message: qsTr("message")
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            Layout.minimumHeight: 200

        ScrollView {
            anchors.fill: parent

            ListView {
                id: logView

                model: logModel

                highlightMoveDuration: 0
                highlightMoveVelocity: 0

                delegate: LogDelegate {
                    width: parent.width
                    height: 25

                    time: new Date().toTimeString()
                    tag: model.tag
                    message: model.message
                }
            }
        }
        }
    }

    style: ApplicationWindowStyle {
        background: Rectangle {
            color: "#fcfcfc"
        }
    }
}
