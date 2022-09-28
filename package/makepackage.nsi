; 该脚本使用 易量安装(az.eliang.com) 向导生成
; 安装程序初始定义常量
!define PRODUCT_NAME "ToneGoPartner"
!define PRODUCT_VERSION "0.0.26"
!define PRODUCT_PUBLISHER "RAR Limited"
!define PRODUCT_WEB_SITE "http://www.tonego.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\CrashReport.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; 提升安装程序权限(vista,win7,win8)
RequestExecutionLevel admin

; ------ MUI 现代界面定义 ------
!include "MUI2.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\eliang-install.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\eliang-uninstall.bmp"
!define MUI_ICON "E:\Work\make-package\installer.ico"
!define MUI_UNICON "E:\Work\make-package\uninstaller.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\eliang-install.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\eliang-uninstall.bmp"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
!insertmacro MUI_PAGE_LICENSE "E:\Work\make-package\license.txt"
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!define MUI_FINISHPAGE_RUN "$INSTDIR\ToneGoPartner.exe"
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "TradChinese"

; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "ToneGoPartner-0.0.26.exe"
;ELiangID 统计编号     /*  安装统计项名称：【ToneGoPartner】  */
InstallDir "$PROGRAMFILES\ToneGoPartner"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUninstDetails show
BrandingText " "

;安装包版本号格式必须为x.x.x.x的4组整数,每组整数范围0~65535,如:2.0.1.2
;若使用易量统计,版本号将用于区分不同版本的安装情况,此时建议用户务必填写正确的版本号
;!define INSTALL_VERSION "2.0.1.2"

;VIProductVersion "${INSTALL_VERSION}"
;VIAddVersionKey /LANG=${LANG_TradChinese} "ProductName"      "ToneGoPartner"
;VIAddVersionKey /LANG=${LANG_TradChinese} "Comments"         "ToneGoPartner(RAR Limited)"
;VIAddVersionKey /LANG=${LANG_TradChinese} "CompanyName"      "RAR Limited"
;VIAddVersionKey /LANG=${LANG_TradChinese} "LegalCopyright"   "RAR Limited(http://www.tonego.com)"
;VIAddVersionKey /LANG=${LANG_TradChinese} "FileDescription"  "ToneGoPartner"
;VIAddVersionKey /LANG=${LANG_TradChinese} "ProductVersion"   "${INSTALL_VERSION}"
;VIAddVersionKey /LANG=${LANG_TradChinese} "FileVersion"      "${INSTALL_VERSION}"

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\CrashReport.exe"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\db.dll"
  SetOutPath "$INSTDIR\imageformats"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\imageformats\qgif4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\imageformats\qjpeg4.dll"
  SetOutPath "$INSTDIR"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\intl.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\MediaCore.dll"
  SetOutPath "$INSTDIR\mediaservice"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\mediaservice\dsengine.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\mediaservice\qtmedia_audioengine.dll"
  SetOutPath "$INSTDIR"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\Microsoft.VC90.CRT.manifest"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\msvcm90.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\msvcp90.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\msvcr90.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\pref.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtCore4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtGui4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtMultimedia4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtMultimediaKit1.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtNetwork4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtOpenGL4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtSql4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtSystemInfo1.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\QtXml4.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\service.dll"
  SetOutPath "$INSTDIR\sqldrivers"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\sqldrivers\qsqlite4.dll"
  SetOutPath "$INSTDIR"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\ToneGoPartner.exe"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\ui.dll"
  File "E:\Work\make-package\ToneGoPartner-0.0.26-win32\utility.dll"

  CreateDirectory "$SMPROGRAMS\ToneGoPartner"
  CreateShortCut "$SMPROGRAMS\ToneGoPartner\ToneGoPartner.lnk" "$INSTDIR\ToneGoPartner.exe"
  CreateShortCut "$DESKTOP\ToneGoPartner.lnk" "$INSTDIR\ToneGoPartner.exe"
SectionEnd

Section -AdditionalIcons
  WriteINIStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\ToneGoPartner\訪問ToneGoPartner主頁.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\ToneGoPartner\卸載ToneGoPartner.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\CrashReport.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\CrashReport.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
*  以下是安装程序的卸载部分  *
******************************/

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\CrashReport.exe"
  Delete "$INSTDIR\db.dll"
  Delete "$INSTDIR\imageformats\qgif4.dll"
  Delete "$INSTDIR\imageformats\qjpeg4.dll"
  Delete "$INSTDIR\intl.dll"
  Delete "$INSTDIR\MediaCore.dll"
  Delete "$INSTDIR\mediacore.txt"
  Delete "$INSTDIR\mediaservice\dsengine.dll"
  Delete "$INSTDIR\mediaservice\qtmedia_audioengine.dll"
  Delete "$INSTDIR\Microsoft.VC90.CRT.manifest"
  Delete "$INSTDIR\msvcm90.dll"
  Delete "$INSTDIR\msvcp90.dll"
  Delete "$INSTDIR\msvcr90.dll"
  Delete "$INSTDIR\pref.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtMultimedia4.dll"
  Delete "$INSTDIR\QtMultimediaKit1.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtSystemInfo1.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\service.dll"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\ToneGoPartner.exe"
  Delete "$INSTDIR\ui.dll"
  Delete "$INSTDIR\utility.dll"

  Delete "$SMPROGRAMS\ToneGoPartner\訪問ToneGoPartner主頁.lnk"
  Delete "$SMPROGRAMS\ToneGoPartner\卸載ToneGoPartner.lnk"
  Delete "$SMPROGRAMS\ToneGoPartner\ToneGoPartner.lnk"
  Delete "$DESKTOP\ToneGoPartner.lnk"

  RMDir "$SMPROGRAMS\ToneGoPartner"

  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\mediaservice"
  RMDir "$INSTDIR\sqldrivers"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
SectionEnd

/* 根据 NSIS 脚本编辑规则,所有 Function 区段必须放置在 Section 区段之后编写,以避免安装程序出现未可预知的问题. */

Function un.onInit
FunctionEnd

Function un.onUninstSuccess
FunctionEnd
