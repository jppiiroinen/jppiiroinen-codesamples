/*****************************************************************************
THIS IS A CODE SAMPLE.

##############################
# (C) 2015 Juhapekka Piiroinen
# All Rights Reserved.
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################
******************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import fi.leetsoftware.cute 1.0
import QtQuick.Controls.Styles 1.1

FocusScope {
    id: settingsDialog

    // make sure that nothing goes thru
    MouseArea {
        anchors.fill: parent
    }

    function close() {
        state = "_CLOSED_";
        parent.forceActiveFocus();
    }

    property bool is_open : state === "_OPEN_"

    function show() {
        state = "_OPEN_";
        header.forceActiveFocus();
    }

    enabled: state === "_OPEN_"

    state: "_CLOSED_"

    Rectangle  {
        anchors.fill: flickable
        color: sysPalette.window
    }

    Flickable {
        id: flickable
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: parent.state === "_OPEN_" ? parent.height : 0

        Behavior on height {
            NumberAnimation {}
        }

        contentWidth: parent.width
        contentHeight: settingsContainer.height
        clip: true

        Item {
            id: settingsContainer

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                topMargin: 10
            }

            height: content.height

            Column {
                id: content
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                spacing: 20
                width: parent.width


                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 5

                    Label {
                        text: qsTr("Show the technical details?")
                        font.weight: Font.Light
                        font.pointSize: 9
                        anchors.horizontalCenter: parent.horizontalCenter
                    }


                    Row {
                        spacing: 10
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            text: qsTr("No, I want to go easy.")
                            opacity: fileSystemSupport.technical_ui ? 0.5 : 1
                        }

                        Switch {
                            checked: fileSystemSupport.technical_ui
                            onCheckedChanged: {
                                fileSystemSupport.technical_ui = checked;
                            }
                        }
                        Label {
                            text: qsTr("Y34h! Br1ng 17 0n")
                            opacity: !fileSystemSupport.technical_ui ? 0.5 : 1
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 3

                    Label {
                        text: qsTr("Sort files and directories")
                        font.weight: Font.Light
                        font.pointSize: 9
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10

                        Label {
                            text: qsTr("No")
                            opacity: !fileSystemSupport.sorted ? 1 : 0.5
                        }

                        Switch {
                            checked: fileSystemSupport.sorted
                            onCheckedChanged: {
                                fileSystemSupport.sorted = checked;
                            }
                        }

                        Label {
                            text: qsTr("Yes")
                            opacity: fileSystemSupport.sorted ? 1 : 0.5
                        }
                    }
                }

                Item {
                    visible: height > 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: fileSystemSupport.sorted && fileSystemSupport.technical_ui ? sortedCol.height + 40 : 0
                    width: parent.width
                    clip: true

                    Behavior on height {
                        NumberAnimation {}
                    }

                    Rectangle {
                        anchors.fill: parent

                        gradient: Gradient {
                            GradientStop {
                                position: -0.5
                                color: Qt.darker(sysPalette.window)
                            }
                            GradientStop {
                                position: 0.05
                                color: sysPalette.window
                            }
                            GradientStop {
                                position: 0.95
                                color: Qt.lighter(sysPalette.window)
                            }

                            GradientStop {
                                position: 1.5
                                color: Qt.darker(sysPalette.window)
                            }
                        }
                    }

                    Column {
                        id: sortedCol
                        anchors {
                            top: parent.top
                            left: parent.left
                            right: parent.right
                            margins: 20
                        }
                        spacing: 10

                        Column {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 3

                            Label {
                                text: qsTr("Sort files by suffix")
                                font.weight: Font.Light
                                font.pointSize: 9
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 10

                                Label {
                                    text: qsTr("No")
                                    opacity: !fileSystemSupport.order_type ? 1 : 0.5
                                }
                                Switch {
                                    checked: fileSystemSupport.order_type
                                    onCheckedChanged: {
                                        fileSystemSupport.order_type = checked;
                                    }
                                }
                                Label {
                                    text: qsTr("Yes")
                                    opacity: fileSystemSupport.order_type ? 1 : 0.5
                                }
                            }
                        }

                        Column {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 3

                            Label {
                                text: qsTr("Sort order")
                                font.weight: Font.Light
                                font.pointSize: 9
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 10

                                Label {
                                    text: qsTr("Descending")
                                    opacity: !fileSystemSupport.order_asc ? 1 : 0.5
                                }

                                Switch {
                                    checked: fileSystemSupport.order_asc
                                    onCheckedChanged: {
                                        fileSystemSupport.order_asc = checked;
                                    }
                                }

                                Label {
                                    text: qsTr("Ascending")
                                    opacity: fileSystemSupport.order_asc ? 1 : 0.5
                                }
                            }
                        }

                        Column {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 10

                            Column {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 5

                                Label {
                                    text: qsTr("Separate directories from files")
                                    font.weight: Font.Light
                                    font.pointSize: 9
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }

                                Row {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    spacing: 10

                                    Label {
                                        text: qsTr("No")
                                        opacity: !separateDirectoriesFromFiles.checked ? 1 : 0.5
                                    }

                                    Switch {
                                        id: separateDirectoriesFromFiles
                                        //anchors.horizontalCenter: parent.horizontalCenter
                                        Component.onCompleted: checked = fileSystemSupport.order_dirs_last || fileSystemSupport.order_dirs_first
                                        onCheckedChanged: {
                                            if (checked) {
                                                if (!fileSystemSupport.order_dirs_last && !fileSystemSupport.order_dirs_last) {
                                                    fileSystemSupport.order_dirs_first = true;
                                                }
                                            }
                                        }
                                    }

                                    Label {
                                        text: qsTr("Yes")
                                        opacity: separateDirectoriesFromFiles.checked ? 1 : 0.5
                                    }
                                }
                            }

                            Column {
                                anchors.horizontalCenter: parent.horizontalCenter
                                visible: separateDirectoriesFromFiles.checked
                                spacing: 5

                                Label {
                                    text: qsTr("Show directories")
                                    font.weight: Font.Light
                                    font.pointSize: 9
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }

                                Row {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    spacing: 10

                                    Label {
                                        text: qsTr("First")
                                        opacity: fileSystemSupport.order_dirs_first ? 1 : 0.5
                                    }

                                    Switch {
                                        Component.onCompleted: checked = fileSystemSupport.order_dirs_last && !fileSystemSupport.order_dirs_first
                                        onCheckedChanged: {
                                            if (checked) {
                                                fileSystemSupport.order_dirs_last = true;
                                                fileSystemSupport.order_dirs_first = false;
                                            } else {
                                                fileSystemSupport.order_dirs_last = false;
                                                fileSystemSupport.order_dirs_first = true;
                                            }
                                        }
                                    }
                                    Label {
                                        text: qsTr("Last")
                                        opacity: fileSystemSupport.order_dirs_last ? 1 : 0.5
                                    }
                                }
                            }
                        }


                        Column {
                            visible: fileSystemSupport.sorted
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 3
                            //height: parent.height

                            Label {
                                text: qsTr("Sort files and directories by")
                                font.weight: Font.Light
                                font.pointSize: 9
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 10

                                ExclusiveGroup {
                                    id: tabSortBy
                                    onCurrentChanged: {
                                        fileSystemSupport.sort_by = current.objectName;
                                    }
                                }

                                RadioButton {
                                    objectName: "SortBy_Name"
                                    text: qsTr("Name")
                                    checked: true
                                    exclusiveGroup: tabSortBy
                                }

                                RadioButton {
                                    objectName: "SortBy_Size"
                                    text: qsTr("Size")
                                    exclusiveGroup: tabSortBy
                                }

                                RadioButton {
                                    objectName: "SortBy_Modified"
                                    text: qsTr("Modified")
                                    exclusiveGroup: tabSortBy
                                }
                            }
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 5

                    Label {
                        text: qsTr("Group files and directories by Date")
                        font.weight: Font.Light
                        font.pointSize: 9
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        spacing: 10
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            text: qsTr("No")
                            opacity: !fileSystemSupport.group_by_date ? 1 : 0.5
                        }

                        Switch {
                            checked: fileSystemSupport.group_by_date
                            onCheckedChanged: {
                                fileSystemSupport.group_by_date = checked;
                            }
                        }

                        Label {
                            text: qsTr("Yes")
                            opacity: fileSystemSupport.group_by_date ? 1 : 0.5
                        }
                    }
                }

                Item {
                    visible: height > 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: fileSystemSupport.group_by_date ? colGroupByDate.height + 40 : 0
                    width: parent.width
                    clip: true

                    Behavior on height {
                        NumberAnimation {}
                    }

                    Rectangle {
                        anchors.fill: parent

                        gradient: Gradient {
                            GradientStop {
                                position: -0.5
                                color: Qt.darker(sysPalette.window)
                            }
                            GradientStop {
                                position: 0.05
                                color: sysPalette.window
                            }
                            GradientStop {
                                position: 0.95
                                color: Qt.lighter(sysPalette.window)
                            }

                            GradientStop {
                                position: 1.5
                                color: Qt.darker(sysPalette.window)
                            }
                        }
                    }

                    Column {
                        id: colGroupByDate
                        anchors {
                            top: parent.top
                            left: parent.left
                            right: parent.right
                            margins: 20
                        }
                        spacing: 10

                        Column {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 5

                            Label {
                                text: qsTr("The most recent files and directories first")
                                font.weight: Font.Light
                                font.pointSize: 9
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Row {
                                spacing: 10
                                anchors.horizontalCenter: parent.horizontalCenter

                                Label {
                                    text: qsTr("No")
                                    opacity: !fileSystemSupport.group_by_date_latest ? 0.5 : 1
                                }

                                Switch {
                                    checked: fileSystemSupport.group_by_date_latest
                                    onCheckedChanged: {
                                        fileSystemSupport.group_by_date_latest = checked;
                                    }
                                }

                                Label {
                                    text: qsTr("Yes")
                                    opacity: fileSystemSupport.group_by_date_latest ? 0.5 : 1
                                }
                            }
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 5
                    visible: fileSystemSupport.technical_ui

                    Label {
                        text: qsTr("Hidden files and directories are")
                        font.weight: Font.Light
                        font.pointSize: 9
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        spacing: 10
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            text: qsTr("Hidden")
                            opacity: fileSystemSupport.show_hidden ? 0.5 : 1
                        }

                        Switch {
                            checked: fileSystemSupport.show_hidden
                            onCheckedChanged: {
                                fileSystemSupport.show_hidden = checked;
                            }
                        }

                        Label {
                            text: qsTr("Shown")
                            opacity: !fileSystemSupport.show_hidden ? 0.5 : 1
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 5
                    visible: fileSystemSupport.technical_ui

                    Label {
                        text: qsTr("The total disk usage of directories is")
                        font.weight: Font.Light
                        font.pointSize: 9
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        spacing: 10
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            text: qsTr("Not Calculated")
                            opacity: fileSystemSupport.calculate_total ? 0.5 : 1
                        }

                        Switch {
                            checked: fileSystemSupport.calculate_total
                            onCheckedChanged: {
                                fileSystemSupport.calculate_total = checked;
                            }
                        }
                        Label {
                            text: qsTr("Calculated")
                            opacity: !fileSystemSupport.calculate_total ? 0.5 : 1
                        }
                    }
                }
            }
        }
    }


    Column {
        visible: parent.state === "_OPEN_"
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 10
        }

        Label {
            text: "Source Code is available in LGPLv3 or Commercial license."
            font.weight: Font.Light
            font.pointSize: 8
            anchors.right: parent.right
        }

        Label {
            text: "Author"
            textFormat: Text.RichText
            font.weight: Font.Light
            font.pointSize: 9
            anchors.right: parent.right
        }

        Label {
            text: "Juhapekka Piiroinen <jp@1337.fi>"
            font.weight: Font.Light
            font.pointSize: 8
            anchors.right: parent.right
        }

        Label {
            textFormat: Text.RichText
            text: "&copy; 2014 | " + app.organizationName + " (" + app.organizationDomain + ") | " + app.applicationDisplayName + " " + app.applicationVersion
            font.weight: Font.Light
            font.pointSize: 8
             anchors.right: parent.right
        }
    }
}
