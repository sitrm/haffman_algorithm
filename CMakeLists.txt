cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Haffman)
set(NAME_LIB HaffmanLib)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)          # переменная - путь до заголовков 

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)                    # путь до каталога с библиотеками 

set(SOURCE_CODE  main.cpp)    

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

project(${PROJECT_NAME} LANGUAGES CXX)


add_subdirectory(./lib)
 # Указываем пути к заголовочным файлам
include_directories(${INCLUDE_DIR})

#-------------------------------------------------------


#Указываем пути к библиотекам
# #link_directories(SOURCE_LIB)
# set_target_properties(${NAME_LIB}  PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

# исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCE_CODE})   

# Подключаем динамическую библиотеку
target_link_libraries(${PROJECT_NAME} ${NAME_LIB})


#-----------------------------------------------