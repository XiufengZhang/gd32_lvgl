C_SOURCES += $(wildcard $(DRI_DIR)/src/*.c)

DEPPATH += --dep-path $(DRI_DIR)/src
# Makefile文件中的特殊变量,作用在在依赖文件上,如果当前目录找不到就到VPATH下去找 
VPATH += :$(DRI_DIR)/src

C_INCLUDES += -I$(DRI_DIR)/inc