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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/googleComputeEngine.js" as GCE
import "js/support/helpers.js" as Helpers
import "js/support/jsVars.js" as JS


Item {
    id: pageAuthorized
    anchors.fill: parent
    property bool _initialized : (_initialized_me && _initialized_instancemodel)
    property bool _initialized_me : false
    property bool _initialized_instancemodel : false
    property bool _initialized_zonesmodel : false
    property bool _initialized_hdd : false

    SplitView {
        id: layout
        anchors.fill: parent
        //enabled: _initialized


        Component.onCompleted: {
            console.log("GCE CURRENT TOKEN: " + GCE.current_auth_token());
            GCE.getCurrentUser(function(status, response) {
               if (status === 200) {
                  // console.log(JSON.stringify(response));
                   JS.person = response;
                   emailAddress.text = JS.person.emails[0].value;
                   displayName.text = JS.person.displayName;
                   profileImg.source = JS.person.image.url;
                   _initialized_me = true;


               } else {
                   if (GCE.parseError(response)) {
                       GCE.logout();
                       appMain.state = "LOGOUT";
                   }
               }
            });

        }


        Rectangle {
            id: rightToolbar
            color: "#fff" //'#cbcbcb'

            Layout.fillHeight: true
            Layout.minimumWidth: 50
            Layout.preferredWidth: 100
            Layout.maximumWidth: 300
            Layout.minimumHeight: 150
            clip: true
            Label {
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    margins: 10
                }
                visible: parent.width > 200

                text: "Google Developers Console"
                font.weight: Font.Light
                color: "blue"
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Qt.openUrlExternally("https://console.developers.google.com");
                    }
                }
            }
            Behavior on width {
                NumberAnimation {}
            }

            Column {
             //   opacity: _initialized_me ? 1 : 0.5
                anchors {
                   top: parent.top
                   left: parent.left
                   right: parent.right
                }

                spacing: 10

                Row {
                   spacing: 10

                   Image {
                       id: profileImg
                       width: 50
                       height: width
                       fillMode: Image.PreserveAspectFit
                       MouseArea {
                           anchors.fill: parent
                           onClicked: {
                               rightToolbar.width = rightToolbar.width===50 ? 350 : 50
                           }
                       }

                       BusyIndicator {
                           anchors.centerIn: parent
                           visible: !_initialized_me
                           running: visible
                           width: 40
                           height: width
                       }
                   }

                   Column {

                       anchors.verticalCenter: parent.verticalCenter
                       width: 190
                       clip: true

                       Label {
                          id: displayName
                          width: parent.width
                          font.pointSize: 9

                       }

                       Label {
                          id: emailAddress
                          width: parent.width
                          font.pointSize: 8
                       }
                   }

                   Image {
                       source: "img/glyphicons_387_log_out.png"
                        anchors.verticalCenter: parent.verticalCenter
                       width: 30

                       height: width

                       MouseArea {
                           anchors.fill: parent
                          onClicked: {
                              GCE.logout();
                              appMain.state = "LOGOUT";
                          }
                       }
                   }
                }

                Row {
                   spacing: 20
                   anchors {
                       left: parent.left
                       leftMargin: 10
                       right: parent.right
                   }

                   Image {
                       id: serverImg
                       width: 50 - 20

                       height: width
                       fillMode: Image.PreserveAspectFit
                       source: "img/glyphicons_463_server.png"



                       MouseArea {
                           anchors.fill: parent
                           onClicked: {
                               currentPage.changeTo("PageAuthorizedInstance.qml");

                           }
                       }
                   }


                   Label {
                       text: "Instances"
                       anchors.verticalCenter: parent.verticalCenter
                       font.weight: Font.Light
                       font.pointSize: 14
                   }

                }
                Row {
                   spacing: 20
                   anchors {
                       left: parent.left
                       leftMargin: 10
                       right: parent.right
                   }
                  // enabled: false
                   opacity: enabled ? 1 : 0.2

                   Image {
                       id: hddImg
                       width: 50 - 20

                       height: width
                       fillMode: Image.PreserveAspectFit
                       source: "img/glyphicons_342_hdd.png"


                       MouseArea {
                           anchors.fill: parent
                           onClicked: {
                               currentPage.changeTo("PageAuthorizedHdd.qml");

                           }
                       }
                   }

                   Label {
                       text: "Disks"
                       anchors.verticalCenter: parent.verticalCenter
                       font.weight: Font.Light
                       font.pointSize: 14
                   }
                }
            }
        }

        Loader {
            id: currentPage
            Layout.fillWidth: true
            Layout.fillHeight: true
             clip: true

            property string _currentPageFileName : ""


            function changeTo(page) {
                console.log(">> changeTo " + page);
                if (page === _currentPageFileName) {
                    currentPage.item.show();
                } else {
                    _currentPageFileName = page;
                    currentPage.source = page;
                }
            }

            onStatusChanged: {
                console.log("status changed to " + status);
                if (status === Loader.Ready) {

                        currentPage.item.init(function() {
                            currentPage.item.show();
                        });

                }

            }
        }
    }

}
