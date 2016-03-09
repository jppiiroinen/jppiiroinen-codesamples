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
.pragma library

.import "support/httpRequest.js" as HTTP
.import "support/componentLoader.js" as LoginDialog
.import "support/localStorage.js" as DB
.import "support/helpers.js" as Helpers

var _initialized = false;
var _sessionActive = false;

function init(callback) {
     console.log(new Date().getTimezoneOffset());
    console.log(" >> GCE Init");
    DB.init("GCE","1.0","Storage for GCE", function() {
            DB.create_table_if_not_exists("Authentication", {
                                              "access_token" : "TEXT",
                                              "expires_in" : "TEXT",
                                              "token_type" : "TEXT",
                                              "refresh_token" : "TEXT",
                                              "id_token" : "TEXT"
                                          }, function() {
                getAuthenticationTokens(function(status, result) {
                    if (status === 200) {
                        var latest_token = result[0];

                        var ts = new Date().getTime();

                        var epochSecs =ts + new Date().getTimezoneOffset()*60*1000;
                        var sessionDurationInSec = (epochSecs - new Date(latest_token._created).getTime()) / 1000

                        console.log("Session is " + sessionDurationInSec +" old");
                        if (sessionDurationInSec < latest_token.expires_in) {
                            HTTP.authorizationToken = latest_token.access_token;
                            _sessionActive = true;
                            console.log(" * session is valid")
                            console.log(" * GCE is now initialized.");
                            _initialized = true;
                            if (callback) callback();
                        } else {
                            _sessionActive = false;
                            console.log(" * session had expired.. clearing session data..");
                            clearAuthenticationTokens(function() {
                                console.log(" * GCE is now initialized.");
                                _initialized = true;
                                if (callback) callback();
                            });
                        }
                    } else {
                        _sessionActive = false;

                        console.log(" * There was no active sessions.");
                        console.log(" * GCE is now initialized.");
                        _initialized = true;
                        if (callback) callback();
                    }


                });
           });

    });
}

function current_auth_token() {
    return HTTP.authorizationToken;
}

function logout() {
    if (LoginDialog.obj) {
        LoginDialog.obj.destroy();
    }
    DB.delete_from("Authentication",{});
    _sessionActive = false;
}

function openLoginDialog(parent, callback) {
    LoginDialog.parent = parent;
    LoginDialog.load("LoginDialog", {}, function(loadResult) {
        if (loadResult.result === 200) {

            LoginDialog.obj.login(function(res) {

                var token = res.payload;
                if (res.result === 200) {
                  //  console.log("Updating access token..");
                   // console.log(JSON.stringify(token));
                    HTTP.authorizationToken = token.access_token;
                    _sessionActive = true;
                } else {
                    _sessionActive = false;
                    console.log("FAILED TO AUTHENTICATE!");
                }

                DB.delete_from("Authentication",{}, function() {
                    if (token !== undefined && token.access_token !== undefined) {
                       // console.log(JSON.stringify(token));
                        DB.insert("Authentication",token);
                    }
                });

                LoginDialog.obj.hide();
                if (callback) callback(res);
            });
        } else {
            console.log("FAILED TO LOAD COMPONENT");
            if (callback) callback(loadResult);
        }
    });
}


function getAuthenticationTokens(callback) {
    DB.select_all("Authentication",function(rows) {
        if (rows.length === 0) {
            callback( 400, "__EMPTY__" );
        } else {
            callback( 200, rows );
        }
    });
}

function clearAuthenticationTokens(callback) {
    DB.delete_from("Authentication",{},function() {
        callback();
    });
}

function isValidSession() {
    var retval = true;
    if (!_initialized) {
        console.log("You must call init first!");

        retval = false
    } else if (!_sessionActive) {
        console.log("No active session");
        retval = false;
    }

    return retval;
}

function checkSession(callback) {
    if (!isValidSession) {
        if (callback) callback(400, "__INVALID_SESSION__");
        return false;
    }
    return true;
}

function getCurrentUser(callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/plus/v1/people/me", {}, function(status,response) {
       if (callback) callback(status, response);
    });
}

function listBackendServices(project, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/global/backendServices", {}, function(status,response) {
       if (callback) callback(status, response);
    });
}


function getProject(project, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project, {}, function(status,response) {
       if (callback) callback(status, response);
    });
}

function parseError(response) {
    var errorList = "";
    var wasAuthError = false;
    var errors = response.error.errors;
    console.log(JSON.stringify(errors));
    for (var i=0; i<errors.length; i++) {
        var error = errors[i];
         errorList += qsTr("%0:%1:%2").arg(error.domain).arg(error.reason).arg(error.message) + "\n";
        if (error.reason === "authError") wasAuthError = true;
    }

    Helpers.showError("Error", response.error.message, errorList);

    return wasAuthError;
}

function listOperations(project, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/aggregated/operations", params, function(status,response) {
       if (callback) callback(status, response );
    });
}

function listInstances(project, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/aggregated/instances", params, function(status,response) {
       if (callback) callback(status, response );
    });
}

function listMachineTypes(project, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/aggregated/machineTypes", params, function(status,response) {
       if (callback) callback(status, response);
    });
}

function listNetworks(project, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/global/networks", params, function(status,response) {
       if (callback) callback(status,response);
    });
}

function listZones(project, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones", params, function(status,response) {
       if (callback) callback(status, response);
    });
}

function listInstancesInZone(project, zone, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/instances", params, function(status,response) {
       if (callback) callback(status,response );
    });
}

function listMachineTypesInZone(project, zone, params, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/machineTypes", params, function(status,response) {
       if (callback) callback(status, response);
    });
}

function getMachineTypeInZone(project, zone, machineType, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/machineTypes/" + machineType, {}, function(status,response) {
       if (callback) callback(status, response);
    });
}

function getOperation(project, zone, operationName, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/operations/" + operationName, {}, function (status, response) {
            if (callback) callback(status, response);
    });
}


function getInstance(project, zone, instanceName, callback) {
    if (!checkSession(callback)) return;

    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/instances/" + instanceName, {}, function (status, response) {
            if (callback) callback(status, response);
    });
}


function createDisk(project, zone, diskName, diskDescription, diskImageUrl, callback) {

        if (!checkSession(callback)) return;

        var diskData = {
            "zone": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone,
            "type": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/diskTypes/pd-standard",
            "name": diskName,
            "description": diskDescription
        }

        var diskImage = encodeURIComponent(diskImageUrl);
        console.log("creating " + diskImage);
                //
        // lets create the boot disk

        HTTP._postDataInJson = true;
        HTTP.http_post("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/disks?sourceImage=" + diskImage, diskData, function (status, response) {
            callback(status,response);
        });
}

function createInstance(project, zone, machineType, instanceName, sshKeys, firewall, callback) {
    if (!checkSession(callback)) return;

    var items = firewall;
    var metaData = [];

    if (sshKeys !== "") {
        metaData.push({ "key": "sshKeys", "value": sshKeys });
    }

    var instanceData = {
        "disks": [
          {
            "type": "PERSISTENT",
            "boot": true,
            "mode": "READ_WRITE",
            "deviceName": instanceName,
            "zone": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone,
            "source": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/disks/" + instanceName,
            "autoDelete": true
          }
        ],
        "networkInterfaces": [
          {
            "network": "https://www.googleapis.com/compute/v1/projects/" + project + "/global/networks/default",
            "accessConfigs": [
              {
                "name": "External NAT",
                "type": "ONE_TO_ONE_NAT"
              }
            ]
          }
        ],
        "metadata": {
          "items": metaData
        },
        "tags": {
            "items": items
        },
        "zone": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone,
        "canIpForward": false,
        "scheduling": {
          "automaticRestart": true,
          "onHostMaintenance": "MIGRATE"
        },
        "machineType": "https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/machineTypes/" + machineType,
        "name": instanceName,
        "serviceAccounts": [
          {
            "email": "default",
            "scopes": [
              "https://www.googleapis.com/auth/devstorage.read_only"
            ]
          }
        ]
      };

    // lets create the instance
    HTTP.http_post("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/instances", instanceData, function(status, response) {
      //  console.log(JSON.stringify(response));
       // console.log(JSON.stringify(status));
        //TODO: if this failed, delete the created disk too
        callback(status, response);
    });
}

function deleteInstance(project, zone, instance, callback) {
    HTTP.http_delete("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/instances/" + instance, {}, callback);
}

function listDisks(project, callback) {
    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/aggregated/disks", {}, callback);
}

function listAllImages(project, callback) {
    var image_projects = [ "debian-cloud", "centos-cloud", "opensuse-cloud", "rhel-cloud", project];

    var images = [];
    var counter = 0;
    var abort = false;
    for (var i=0; i < image_projects.length; i++) {
        if (abort) break;

        listImages(image_projects[i],function(status, result) {
            if (status === 0) {
                abort = true;
                callback(status, result);
                return;
            }

            counter+=1;
            images.push(result);
            if (counter === image_projects.length) {
                images.sort(function(a,b) {
                    if (a.name > b.name) {
                        return 1;
                      }
                      if (a.name < b.name) {
                        return -1;
                      }
                      // a must be equal to b
                      return 0;
                });
                callback(status, images);
            }
        });
    }
}

function listImages(project, callback) {
    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/global/images", {}, callback);
}

function getDisk(project, zone, disk, callback) {
    HTTP.http_get("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/disks/" + disk, {}, callback);
}

function deleteDisk(project, zone, disk, callback) {
    HTTP.http_delete("https://www.googleapis.com/compute/v1/projects/" + project + "/zones/" + zone + "/disks/" + disk, {}, callback);
};
