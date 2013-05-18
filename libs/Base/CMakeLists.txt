enable_qt()

qt4_wrap_cpp(MOC_SRCS
  base_model.h
  tasks_handler.h
)

add_library(duokan_base ${MOC_SRCS}
  base_model.cpp
  tasks_handler.cpp
)
target_link_libraries(duokan_base
  ${QT_LIBRARIES}
  ${ADD_LIB}
)
strict_warning(duokan_base)
