cmake_minimum_required(VERSION 3.1.0)

add_library(restbed)
target_sources(restbed
	PRIVATE
		4.8/corvusoft/restbed/byte.hpp
		4.8/corvusoft/restbed/common.hpp
		4.8/corvusoft/restbed/context_placeholder.hpp
		4.8/corvusoft/restbed/context_placeholder_base.hpp
		4.8/corvusoft/restbed/context_value.hpp
		4.8/corvusoft/restbed/http.cpp
		4.8/corvusoft/restbed/http.hpp
		4.8/corvusoft/restbed/logger.hpp
		4.8/corvusoft/restbed/request.cpp
		4.8/corvusoft/restbed/request.hpp
		4.8/corvusoft/restbed/resource.cpp
		4.8/corvusoft/restbed/resource.hpp
		4.8/corvusoft/restbed/response.cpp
		4.8/corvusoft/restbed/response.hpp
		4.8/corvusoft/restbed/rule.cpp
		4.8/corvusoft/restbed/rule.hpp
		4.8/corvusoft/restbed/service.cpp
		4.8/corvusoft/restbed/service.hpp
		4.8/corvusoft/restbed/session.cpp
		4.8/corvusoft/restbed/session.hpp
		4.8/corvusoft/restbed/session_manager.cpp
		4.8/corvusoft/restbed/session_manager.hpp
		4.8/corvusoft/restbed/settings.cpp
		4.8/corvusoft/restbed/settings.hpp
		4.8/corvusoft/restbed/ssl_settings.cpp
		4.8/corvusoft/restbed/ssl_settings.hpp
		4.8/corvusoft/restbed/status_code.hpp
		4.8/corvusoft/restbed/string.cpp
		4.8/corvusoft/restbed/string.hpp
		4.8/corvusoft/restbed/uri.cpp
		4.8/corvusoft/restbed/uri.hpp
		4.8/corvusoft/restbed/web_socket.cpp
		4.8/corvusoft/restbed/web_socket.hpp
		4.8/corvusoft/restbed/web_socket_message.cpp
		4.8/corvusoft/restbed/web_socket_message.hpp
		4.8/corvusoft/restbed/detail/http_impl.cpp
		4.8/corvusoft/restbed/detail/http_impl.hpp
		4.8/corvusoft/restbed/detail/ipc_socket_impl.cpp
		4.8/corvusoft/restbed/detail/ipc_socket_impl.hpp
		4.8/corvusoft/restbed/detail/request_impl.hpp
		4.8/corvusoft/restbed/detail/resource_impl.hpp
		4.8/corvusoft/restbed/detail/response_impl.hpp
		4.8/corvusoft/restbed/detail/rule_engine_impl.hpp
		4.8/corvusoft/restbed/detail/rule_impl.hpp
		4.8/corvusoft/restbed/detail/service_impl.cpp
		4.8/corvusoft/restbed/detail/service_impl.hpp
		4.8/corvusoft/restbed/detail/session_impl.cpp
		4.8/corvusoft/restbed/detail/session_impl.hpp
		4.8/corvusoft/restbed/detail/settings_impl.hpp
		4.8/corvusoft/restbed/detail/socket_impl.cpp
		4.8/corvusoft/restbed/detail/socket_impl.hpp
		4.8/corvusoft/restbed/detail/ssl_settings_impl.hpp
		4.8/corvusoft/restbed/detail/uri_impl.hpp
		4.8/corvusoft/restbed/detail/web_socket_impl.cpp
		4.8/corvusoft/restbed/detail/web_socket_impl.hpp
		4.8/corvusoft/restbed/detail/web_socket_manager_impl.cpp
		4.8/corvusoft/restbed/detail/web_socket_manager_impl.hpp
		4.8/corvusoft/restbed/detail/web_socket_message_impl.hpp
)

target_compile_features(restbed PRIVATE cxx_std_23)
if (WIN32)
	target_compile_definitions(restbed PUBLIC WIN_DLL_EXPORT)
	target_compile_definitions(restbed PUBLIC _WIN32_WINNT=0x0601)
	target_compile_options(restbed PUBLIC /wd4251 /wd4996)
endif ()

target_include_directories(restbed PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/4.8)

target_link_libraries(restbed PRIVATE asio)
