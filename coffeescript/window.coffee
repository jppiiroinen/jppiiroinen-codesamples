# THIS IS A CODE SAMPLE.
##############################
# (C) 2015 Juhapekka Piiroinen
# All Rights Reserved.
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################

# https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_Operators

windowPage = {}

# TODO:
# we need a window manager which will track
#  - open windows
#  - currently active window
#  - triggers events on windows, suchs as focus and blur

_ = require 'lodash'
m = require 'mithril'
#ui = require './common/ui'
randomstring = require "randomstring"

windowPage.controller = (props) ->
  validateSession()

  @enums =
    type:
      NORMAL: 0x0 # The window has no special behavior
      MODAL: 0x1 # the window is a modal dialog
      TOPLEVEL: 0x10 # the window is on top of everything
    modality:
      NON_MODAL: 0x0 # The window is not modal and does not block input to other windows.
      WINDOW_MODAL: 0x1 # The window is modal to a single window hierarchy and blocks input to its parent window, all grandparent windows, and all siblings of its parent and grandparent windows.
      APP_MODAL: 0x10 # The window is modal to the application and blocks input to all windows.
    state:
      NORMAL: 0x0
      MINIMIZED: 0x1 # The window is minimized (i.e. iconified).
      MAXIMIZED: 0x10 # The window is maximized with a frame around it.
      FULLSCREEN: 0x100 # The window fills the entire screen without any frame around it.
    rendering :
      NOTSET: 0x0 # the window is not active and not visible.
      ACTIVE: 0x1 # window is active
      VISIBLE: 0x10 # the window is visible and should be rendered

  @init = (props) ->
    props = props or {}
    @props =
      id: m.prop "#{randomstring.generate()}"
      icon: m.prop props.icon or "/images/window/icon.png"
      title: m.prop props.title or "Window Title"
      opacity: m.prop props.opacity or 1
      buttonbox: m.prop props.buttonbox or true
      focus: m.prop props.focus or true
      parent: m.prop props.parent or ".desktop"
      zindex: m.prop props.zindex or 1
      children: m.prop props?.content or []
      pos:
        x: m.prop props.pos?.x or 0
        y: m.prop props.pos?.y or 0
      size:
        width: m.prop props.width or 300
        height: m.prop props.height or 300
        max:
          width: m.prop props.size?.max?.width or 300
          height: m.prop props.size?.max?.height or 300
        min:
          width: m.prop props.size?.min?.width or 300
          height: m.prop props.size?.min?.height or 300
      flags:
        type: m.prop props.flags?.type or 0
        state: m.prop props.flags?.state or 0
        rendering: m.prop props.flags?.rendering or 0
        modality: m.prop props.flags?.modality or 0

  @windowTitle = ->
    @props.title()

  @windowId = ->
    @props.id()

  @windowIcon = ->
    @props.icon()

  @setSize = ({width, height}) ->

    console.log "setSize", width, height
    @props.size.width width
    @props.size.height height


  @parentId = ->
    @props.parent()

  @children = ->
    @props.children()

  # This property holds the active status of the window.
  @isActive = ->
    @props.flags.rendering() & @enums.rendering.ACTIVE

  # Returns whether the window is modal.
  # A modal window prevents other windows from getting any input.
  @isModal = ->
    @props.flags.type() & @enums.type.MODAL

  @isVisible = ->
    @props.flags.rendering() & @enums.rendering.VISIBLE

  # Returns whether the window is top level, i.e. has no parent window.
  @isTopLevel = ->
    @props.flags.type() & @enums.type.TOPLEVEL

  # the window's icon in the windowing system
  @icon = ->
    @props.icon()

  # Hides the window.
  # Equivalent to calling setVisible(false).
  @hide = ->
    mask = ~(@enums.rendering.VISIBLE)
    @props.flags.rendering(@props.flags.rendering() | mask)
    return

  # Shows the window.
  # This is equivalent to calling showFullScreen(),
  # showMaximized(), or showNormal(),
  # depending on the platform's default behavior for the window type and flags.
  @show = ->
    mask = (@enums.rendering.VISIBLE)
    @props.flags.rendering(@props.flags.rendering() | mask)
    return

  # Returns the maximum size of the window.
  @maximumSize = ->
    max = @props.size.max
    [max.width(), max.height()]

  # Returns the minimum size of the window.
  @minimumSize = ->
    min = @props.size.min
    [min.width(), min.height()]

  @modality = ->
    @props.flags.modality()

  @toggleMinimize = =>
    $("##{@props.id()}").zIndex @props.zindex()
    if not @isWindowMinimized()
      @props.flags.state @enums.state.MINIMIZED
    else
      @props.flags.state @enums.state.NORMAL
    @onResize()

  @close = =>
    console.log "##{@props.id()}"
    $("##{@props.id()}").remove()

  @windowWidth = ->
    @props.size.width()

  @windowHeight = ->
    @props.size.height()

  @desktopWidth = ->
    $(@props.parent()).width()

  @desktopHeight = ->
    $(@props.parent()).height()

  @hasButtonBox = ->
    @props.buttonbox()

  @isWindowMinimized = ->
    @props.flags.state() & @enums.state.MINIMIZED

  @isWindowMaximized = ->
    @props.flags.state() & @enums.state.MAXIMIZED

  @toggleMaximize = =>
    el = $("##{@props.id()}")
    if not @isWindowMaximized()
      @props.flags.state  @enums.state.MAXIMIZED
      @props.pos.x el.offset().left
      @props.pos.y el.offset().top
      el.offset
        left: 0
        top: 0
      el.zIndex 999999
    else
      @props.flags.state @enums.state.NORMAL
      el.offset
        left: @props.pos.x
        top: @props.pos.y
      el.zIndex @props.zindex

    @onResize()

  @moveWindow = (pos) ->
    $("##{@props.id()}").offset pos

  @resizeWindow = (size) ->
    $("##{@props.id()}").height size.height
    $("##{@props.id()}").width size.width

  # TODO: save the size when the drag has been released or completed.
  # then this will work as it should be.
  @onDragStop = (e) =>
    console.log "TODO:dragstop:",e

  @onDrag = (e) =>
    if e
      if e.clientY < 1
        newSiz =
          width: @windowWidth()
          height: @desktopHeight()
        
        @setSize newSiz
        @resizeWindow newSiz

      # TODO: There can not be a "dual dock" for windows
      #       we have to check that the other side is not already scaled up.
      #       in other words. Window manager!
      else if e.clientX < 1
        newSiz =
          width: @desktopWidth()
          height: @windowHeight()

        @setSize newSiz
        @resizeWindow newSiz
      else
        @resizeWindow
          width: @windowWidth()
          height: @windowHeight()
    true

  @onResize = (e) =>
    el = $("##{@props.id()}")
    if e
      @resizeWindow
        width: e.clientX
        height: e.clientY
    else
      if @isWindowMinimized()
        console.log "window was minimized"
        el.height el.children(".app-window-header").height()
      else if @isWindowMaximized()
        console.log "window is maximumized"
        @setSize el
        @resizeWindow
          width: @desktopWidth()
          height: @desktopHeight()
      else
        console.log "window is normal"
        @resizeWindow
          width: @windowWidth()
          height: @windowHeight()

  @onWindowConfig = (element, isInitialized, context) =>
    console.log "onWindowConfig"
    if isInitialized
      return

    console.log "windowconfig called"

    # disable context menu for window
    $(element).bind "contextmenu", (e) ->
      false

    # store the window size and location
    console.log $(element)

    @windowWidth $(element).width()
    @windowHeight $(element).height()
    #@windowPosition $(element).offset()
    #@windowZIndex $(element).zIndex()

    # event handler for focus
    $(element).focus ->
      console.log "focus in called"

    $(element).blur ->
      console.log "focus out called"

    $(element).click ->
      console.log "click called"
      $(element).toggleClass "inactive"
      $(element).triggerHandler( "focus" )

    # make element draggable
    $(element).draggable
      handle: ".app-window-header"
      containment: "#{@props.parent()}"
      scroll: false
      snap: true

    # make element resizable
    $(element).resizable
      minHeight: 100
      minWidth: 100
      containment: "#{@props.parent()}"
      ghost: true

    console.log @props

  @

windowPage.view = (ctrl) ->
  console.log "window draw"
  m ".app-window app-window-border " + (if ctrl.isActive() then "app-window-shadow" else ""),
    id: ctrl.windowId()
    onresize: ctrl.onResize
    ondrag: ctrl.onDrag
    ondragstop: ctrl.onDragStop
    config: ctrl.onWindowConfig
    ,[
      m ".app-window-header", [
        if ctrl.hasButtonBox()
          m ".buttonbox", [
            if not ctrl.isWindowMaximized() and not ctrl.isWindowMinimized()
              m ".btn-flat .btn-close",
                  onclick: ctrl.close
                , m "i", class:"fa fa-times"
            if not ctrl.isWindowMaximized()
              m ".btn-flat .btn-minimize",
                  onclick: ctrl.toggleMinimize
                , m "i", class:"fa " + (if ctrl.isWindowMinimized() then "fa-chevron-left" else "fa-chevron-down")
            if not ctrl.isWindowMinimized()
              m ".btn-flat .btn-maximize",
                  onclick: ctrl.toggleMaximize
                , m "i", class:"fa " + (if ctrl.isWindowMaximized() then "fa-chevron-left" else "fa-chevron-up")
          ]
        m ".window-title", ctrl.windowTitle()
        m ".window-icon",
          m "img", src: ctrl.windowIcon()
      ]
      if not ctrl.isWindowMinimized()
        m ".app-window-body",
          m ".app-window-body-content", ctrl.children()
      if not ctrl.isWindowMinimized()
        m ".app-window-footer", [

        ]
  ]

module.exports = windowPage