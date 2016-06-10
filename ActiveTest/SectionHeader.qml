import QtQuick 2.0

Rectangle {
    color: "#efefef"

    property string text

    DesignLabel {
        anchors.fill: parent
        anchors.leftMargin: 15

        text: parent.text
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

        font.pixelSize: 18
        color: "#2f2f2f"
    }
}
