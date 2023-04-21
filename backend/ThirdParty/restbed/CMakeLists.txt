cmake_minimum_required(VERSION 3.1.0)

add_library(asio INTERFACE)

target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/restbed/dependency/asio/asio/include)

add_library(restbed)
target_sources(restbed
	PRIVATE
		restbed/source/corvusoft/restbed/byte.hpp
		restbed/source/corvusoft/restbed/common.hpp
		restbed/source/corvusoft/restbed/context_placeholder.hpp
		restbed/source/corvusoft/restbed/context_placeholder_base.hpp
		restbed/source/corvusoft/restbed/context_value.hpp
		restbed/source/corvusoft/restbed/http.cpp
		restbed/source/corvusoft/restbed/http.hpp
		restbed/source/corvusoft/restbed/logger.hpp
		restbed/source/corvusoft/restbed/request.cpp
		restbed/source/corvusoft/restbed/request.hpp
		restbed/source/corvusoft/restbed/resource.cpp
		restbed/source/corvusoft/restbed/resource.hpp
		restbed/source/corvusoft/restbed/response.cpp
		restbed/source/corvusoft/restbed/response.hpp
		restbed/source/corvusoft/restbed/rule.cpp
		restbed/source/corvusoft/restbed/rule.hpp
		restbed/source/corvusoft/restbed/service.cpp
		restbed/source/corvusoft/restbed/service.hpp
		restbed/source/corvusoft/restbed/session.cpp
		restbed/source/corvusoft/restbed/session.hpp
		restbed/source/corvusoft/restbed/session_manager.cpp
		restbed/source/corvusoft/restbed/session_manager.hpp
		restbed/source/corvusoft/restbed/settings.cpp
		restbed/source/corvusoft/restbed/settings.hpp
		restbed/source/corvusoft/restbed/ssl_settings.cpp
		restbed/source/corvusoft/restbed/ssl_settings.hpp
		restbed/source/corvusoft/restbed/status_code.hpp
		restbed/source/corvusoft/restbed/string.cpp
		restbed/source/corvusoft/restbed/string.hpp
		restbed/source/corvusoft/restbed/uri.cpp
		restbed/source/corvusoft/restbed/uri.hpp
		restbed/source/corvusoft/restbed/web_socket.cpp
		restbed/source/corvusoft/restbed/web_socket.hpp
		restbed/source/corvusoft/restbed/web_socket_message.cpp
		restbed/source/corvusoft/restbed/web_socket_message.hpp
		restbed/source/corvusoft/restbed/detail/http_impl.cpp
		restbed/source/corvusoft/restbed/detail/http_impl.hpp
		restbed/source/corvusoft/restbed/detail/ipc_socket_impl.cpp
		restbed/source/corvusoft/restbed/detail/ipc_socket_impl.hpp
		restbed/source/corvusoft/restbed/detail/request_impl.hpp
		restbed/source/corvusoft/restbed/detail/resource_impl.hpp
		restbed/source/corvusoft/restbed/detail/response_impl.hpp
		restbed/source/corvusoft/restbed/detail/rule_engine_impl.hpp
		restbed/source/corvusoft/restbed/detail/rule_impl.hpp
		restbed/source/corvusoft/restbed/detail/service_impl.cpp
		restbed/source/corvusoft/restbed/detail/service_impl.hpp
		restbed/source/corvusoft/restbed/detail/session_impl.cpp
		restbed/source/corvusoft/restbed/detail/session_impl.hpp
		restbed/source/corvusoft/restbed/detail/settings_impl.hpp
		restbed/source/corvusoft/restbed/detail/socket_impl.cpp
		restbed/source/corvusoft/restbed/detail/socket_impl.hpp
		restbed/source/corvusoft/restbed/detail/ssl_settings_impl.hpp
		restbed/source/corvusoft/restbed/detail/uri_impl.hpp
		restbed/source/corvusoft/restbed/detail/web_socket_impl.cpp
		restbed/source/corvusoft/restbed/detail/web_socket_impl.hpp
		restbed/source/corvusoft/restbed/detail/web_socket_manager_impl.cpp
		restbed/source/corvusoft/restbed/detail/web_socket_manager_impl.hpp
		restbed/source/corvusoft/restbed/detail/web_socket_message_impl.hpp
)

target_compile_features(restbed PRIVATE cxx_std_17)
if (WIN32)
	target_compile_definitions(restbed PUBLIC WIN_DLL_EXPORT)
	target_compile_definitions(restbed PUBLIC _WIN32_WINNT=0x0601)
	target_compile_options(restbed PUBLIC /wd4251 /wd4996)
endif ()

target_include_directories(restbed PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/restbed/source)

target_link_libraries(restbed PRIVATE asio)