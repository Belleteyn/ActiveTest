import QtQuick 2.5
import QtQuick.Layouts 1.1

RowLayout {
    property string time: ""
    property string tag: ""
    property string message: ""

    DesignLabel {
        Layout.minimumWidth: 100
        Layout.fillHeight: true

        text: parent.time

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    DesignLabel {
        Layout.minimumWidth: 50
        Layout.fillHeight: true

        text: parent.tag

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    DesignLabel {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.leftMargin: 10

        text: parent.message

        verticalAlignment: Text.AlignVCenter
    }
}

