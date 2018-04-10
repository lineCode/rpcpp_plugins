install(TARGETS ${PROJECT_NAME} DESTINATION ${RPPLUGIN_INSTALL_DIR})
install(TARGETS ${RPPLUGIN_ID} EXPORT ${TARGET_EXPORT_NAME})
export(EXPORT ${TARGET_EXPORT_NAME}
    NAMESPACE ${TARGET_NAMESPACE}
    FILE "${PROJECT_BINARY_DIR}/${TARGET_EXPORT_NAME}.cmake"
)

install(FILES "${PROJECT_SOURCE_DIR}/config.yaml" DESTINATION ${RPPLUGIN_INSTALL_DIR})
foreach(directory_name "include" "resources" "shader")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/${directory_name}"
        DESTINATION ${RPPLUGIN_INSTALL_DIR}
        OPTIONAL
    )
endforeach()

install(FILES ${PACKAGE_CONFIG_FILE} ${PACKAGE_VERSION_CONFIG_FILE} DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR})
install(FILES ${${PROJECT_NAME}_MACRO_CMAKE_FILE} DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR} OPTIONAL)
install(EXPORT ${TARGET_EXPORT_NAME} NAMESPACE ${TARGET_NAMESPACE} DESTINATION ${PACKAGE_CMAKE_INSTALL_DIR})

install_debugging_information(TARGET ${PROJECT_NAME} DESTINATION "${RPPLUGIN_INSTALL_DIR}")
