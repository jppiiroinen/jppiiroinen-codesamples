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
    id: pageAuthorizedOverview
    Layout.fillHeight: true
    Layout.minimumWidth: 200

    enabled: opacity > 0.5

    function init(callback) {
        console.log(">> init called for overvie");
        instanceModel.updateModel(function() {
             callback();
        });
    }

    function show() {
        hdr.addServerMode = false;
        opacity = 1;
    }

    ListModel  {
        id: instanceModel
        dynamicRoles: true

        function updateModel(callback) {
            console.log("updateModel called!");
            _initialized_instancemodel = false;
            instanceModel.clear();
            GCE.listInstances(appWindow.currentProject,{}, function(status, response) {
                if (status === 0) {
                    GCE.logout();
                    appMain.state = "LOGOUT";
                }

                if (response.kind !== undefined && response.kind === "compute#instanceAggregatedList") {

                   var instance_keys = Object.keys(response.items);
                   var instances = [];

                    if (instance_keys.length > 0) {
                       for (var i=0; i<instance_keys.length; i++) {
                           var keyObj = response.items[instance_keys[i]];
                           var zone = instance_keys[i];



                           if (keyObj.instances !== undefined) {
                               console.log("FOUND INSTANCES: " + JSON.stringify(keyObj));
                               for (var iIns=0; iIns<keyObj.instances.length; iIns++) {
                                   var instance = keyObj.instances[iIns];
                                   instance["key"] = zone;
                                   instances.push(instance);
                               }
                           }
                       }
                    }

                    console.log("INSTANCES AFTER: " + JSON.stringify(instances));

                    for (var j=0; j<instances.length; j++) {
                        console.log("ADDING " + JSON.stringify(instances[j]));
                        instanceModel.append(instances[j]);
                    }
                    _initialized_instancemodel = true;
                    if (callback) callback(status, response);
                }

                if (callback) callback();
            });
        }
    }

    Rectangle {
        id: hdr
       anchors {
           top: parent.top
           left: parent.left
           right: parent.right
       }
       height: 50
       property bool addServerMode : false

       Row {
           anchors {
               left: parent.left
               leftMargin: 20
               verticalCenter: parent.verticalCenter
           }
           spacing: 10


           Image {
               source: "img/glyphicons_464_server_plus.png"
               width: height
               height: hdr.height - 20
               enabled: !hdr.addServerMode
               anchors {
                   verticalCenter: parent.verticalCenter
               }
               MouseArea {
                   anchors.fill: parent
                   onClicked: {
                       hdr.addServerMode = !hdr.addServerMode;
                   }
               }
           }

           Label {
               anchors.verticalCenter: parent.verticalCenter
               visible: !hdr.addServerMode
               text: "Instances"
               font.pixelSize: 20
               font.weight: Font.Light

           }
       }


      Row {
           anchors {
               right: parent.right
               rightMargin: 10

               top: parent.top
               bottom: parent.bottom
               margins: 10
           }
           spacing: 10



           Image {
               source: "img/glyphicons_081_refresh.png"
               width: height
               height: parent.height
               MouseArea {
                   anchors.fill: parent
                   onClicked: {

                       instanceModel.updateModel(function() {

                       });
                   }
               }
           }
       }
    }

    Label {
        anchors.centerIn: parent
        visible: (_initialized_instancemodel && instanceModel.count === 0)
        text: "You have no instances."
        font.weight: Font.Light
    }

    ListView {
        id: listView
        visible: count > 0
        clip: true
        anchors {
            top: hdr.bottom
            left: parent.left
            //leftMargin: 10
            right: parent.right
            bottom: parent.bottom
        }


       // cellHeight: 100
       // cellWidth: parent.width
        onCountChanged: {
            console.log("Instance count is now: " + count);
        }

        model: instanceModel

        delegate: Rectangle {
            height: 100
            anchors {
                left: parent.left
                right: parent.right

            }
            color: "#fff"


            property bool loading : false;
            property real loadingProgress : 0.0
            property string loadingText : ""

            property var item : instanceModel.get(index)

            onItemChanged: {
                if (item.status === "PROVISIONING") {
                    var timer = Helpers.setInterval(function() {
                        console.log(">> lets get an operation status");
                        GCE.getInstance(appWindow.currentProject, item.zone, item.name, function(status, response) {
                            if (status === 200) {
                                instanceModel.set(index,response);
                            }
                        });
                    }, 2000);
                }
            }

            function getNetworkInfo() {
                var retval = "";
                for (var i=0; i<item.networkInterfaces.count; i++) {
                    var networkInterface = item.networkInterfaces.get(i);

                    var accessConfigs = networkInterface.accessConfigs;
                    retval += qsTr("-- %0 --\n").arg(networkInterface.name);
                    retval += qsTr("Intra: %0\n").arg(networkInterface.networkIP);
                    for (var j=0; j<accessConfigs.count; j++) {
                        var accessConfig = accessConfigs.get(j);
                        retval += qsTr("%0: %1").arg(accessConfig.name).arg(accessConfig.natIP);
                    }
                }

                console.log("NETWORKINFO: " + retval);
                return retval;
            }

            Column {
                id: col
                enabled: !loading
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    margins: 10
                }
                spacing: 10

                RowLayout {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    spacing: 10

                    Column {
                        Layout.minimumWidth: 100
                        anchors.verticalCenter: parent.verticalCenter
                        Label {
                            text: item.name
                            font.weight: Font.Light
                        }
                        Label {
                            text: item.status
                            font.weight: Font.Light
                            font.pointSize: 9
                        }
                        Label {
                            text: item.key
                            font.weight: Font.Light
                            font.pointSize: 9
                        }

                        Label {
                            text: item.creationTimestamp
                            font.weight: Font.Light
                            font.pointSize: 8
                        }
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            text: getNetworkInfo()
                            font.weight: Font.Light
                            font.pointSize: 8
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        //visible: instances ? (instances.count > 0) : false
                        source: "img/glyphicons_207_remove_2.png"
                        width: 40
                        height: width
                        opacity: enabled ? 1 : 0.5

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                var splitZone = item.zone.split("/");
                                loading = true;
                                loadingProgress = 0.0;
                                //loadingText = "Deleting instance..";

                                var zone = splitZone[splitZone.length-1];
                                GCE.deleteInstance(appWindow.currentProject, zone, item.name, function(status, response) {

                                    if (status !== 200) {
                                        Helpers.showError("Unable to delete instance",response.error.message, JSON.stringify(response));
                                    } else {
                                        var operationName = response.name;
                                        var timer = Helpers.setInterval(function() {
                                            console.log(">> lets get an operation status");
                                            GCE.getOperation(appWindow.currentProject, zone, operationName, function(status, response) {
                                                console.log("got the operation");
                                                if (status === 200 && response.kind === "compute#operation" && response.status === "DONE") {
                                                    instanceModel.updateModel();
                                                    timer.destroy();
                                                    loadingText = "";
                                                    loading = false;
                                                } else {
                                                    timer.restart();
                                                }
                                                loadingProgress = response.progress;
                                            });
                                        }, 3000);
                                    }
                                });

                            }
                        }
                    }
                }
            }

            Row {
                anchors.centerIn: parent
                visible: loading
                spacing: 10

                BusyIndicator {
                    width: 40
                    height: width
                    running: visible
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                   anchors.horizontalCenter: parent.horizontalCenter

                    text: loadingText
                    visible: text !== ""
                }
            }
        }
    }

    PageAuthorizedInstanceCreate {
        id: pageAuthorizedInstanceCreate
        anchors.fill: parent
        visible: hdr.addServerMode
    }

    BusyIndicator {
        anchors.centerIn: parent
        width: 40
        height: width
        running: visible
        visible: !_initialized_instancemodel
    }


}
