!isEmpty(SMPP_PRI_INCLUDED):error("smpp.pri already included")

SMPP_PRI_INCLUDED = 1

defineReplace(cleanPath) {
  win32:1 ~= s|\\\\|/|g
  contains(1, ^/.*):pfx = /
  else:pfx =
  segs = $$split(1, /)
  out =
  for(seg, segs) {
    equals(seg, ..):out = $$member(out, 0, -2)
    else:!equals(seg, .):out += $$seg
  }
  return($$join(out, /, $$pfx))
}

defineReplace(qtLibraryName) {
  unset(LIBRARY_NAME)
  LIBRARY_NAME = $$1
  CONFIG(debug, debug|release) {
    !debug_and_release|build_pass {
      win32: RET = $$member(LIBRARY_NAME, 0)d
    }
  }
  isEmpty(RET):RET = $$LIBRARY_NAME
  return($$RET)
}

defineTest(minQtVersion) {
  maj = $$1
  min = $$2
  patch = $$3
  isEqual(QT_MAJOR_VERSION, $$maj) {
      isEqual(QT_MINOR_VERSION, $$min) {
          isEqual(QT_PATCH_VERSION, $$patch) {
              return(true)
          }
          greaterThan(QT_PATCH_VERSION, $$patch) {
              return(true)
          }
      }
      greaterThan(QT_MINOR_VERSION, $$min) {
          return(true)
      }
  }
  return(false)
}

win32:i_flag = i
defineReplace(stripSrcDir) {
  win32 {
    !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
  } else {
    !contains(1, ^/.*):1 = $$OUT_PWD/$$1
  }
  out = $$cleanPath($$1)
  out ~= s|^$$re_escape($$PWD/)||$$i_flag
  return($$out)
}

isEmpty(SMPP_VERSION) {
  SMPP_VERSION = 1.0.0
}

isEmpty(APP_TARGET) {
  APP_TARGET = smpp
}

isEmpty(SOURCE_TREE) {
  SOURCE_TREE = $$PWD
}

isEmpty(APP_PATH) {
  APP_PATH = $$SOURCE_TREE/bin
}

isEmpty(LIBRARYES_PATH) {
  LIBRARYES_PATH = $$SOURCE_TREE/bin
}

contains(TEMPLATE, lib)|contains(TEMPLATE, app) {
  debug: {
    OBJECTS_DIR = $${SOURCE_TREE}/tmp/debug/obj
    MOC_DIR = $${SOURCE_TREE}/tmp/debug/moc
  } else: release: {
    OBJECTS_DIR = $${SOURCE_TREE}/tmp/release/obj
    MOC_DIR = $${SOURCE_TREE}/tmp/release/moc
  }
}

INCLUDEPATH += \
    $$SOURCE_TREE/src/libs \
    $$SOURCE_TREE/src/plugins

QT     -= gui core network

CONFIG += depend_includepath

LIBS += -L$$LIBRARYES_PATH
