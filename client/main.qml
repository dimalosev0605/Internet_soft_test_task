import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

import Client_qml 1.0

Window {
    visible: true
    width: 640
    height: 480


    Client {
        id: client
        onResponse_received: {
            lbl.text = response
        }
    }

    Label {
        id: lbl
        anchors.centerIn: parent
        width: parent.width / 2
        height: 30
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        fontSizeMode: Text.Fit
        minimumPointSize: 5
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
    }

}
