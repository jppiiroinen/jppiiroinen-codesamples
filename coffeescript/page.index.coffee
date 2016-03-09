# THIS IS A CODE SAMPLE.
##############################
# (C) 2015 Juhapekka Piiroinen
# All Rights Reserved.
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################

indexPage = {}


_ = require 'lodash'
m = require 'mithril'
ui = require './common/ui'

indexPage.controller = =>
  validateSession()

  [ @windowView, @windowCtrl ] = submodule require './window'
  [ @windowTwoView, @windowTwoCtrl ] = submodule require './window'
  @windowCtrl.init
    title: "I am one"
    children: [
      "Hello content one"
    ]
  
  @windowTwoCtrl.init
    title: "I am two"
    children: [
      "hello content two"
    ]

  @windowTwoCtrl.show()
  @windowCtrl.show()
  console.log "index"
  @

indexPage.view = (ctrl) ->
  m ".desktop", [
    ctrl.windowView()
    ctrl.windowTwoView()
    m ".taskbar", []
  ]

module.exports = indexPage