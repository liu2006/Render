#ifndef PIPELINE_H
#define PIPELINE_H

#include "vulkan.h"

typedef struct {
    size_t spirvSize;
    uint32_t *spirvBuffer;
} Pipeline;

bool createPipeline(Vulkan *vulkan, Pipeline *pipeline, const char *fname);

#if defined(PIPELINE_IMPLEMENTATION)
    bool createPipeline(Vulkan *vulkan, Pipeline *pipeline, const char *fname) 
    {
	FILE *shaderFile;
	shaderFile = fopen(fname, "rb");
	if (!shaderFile) {
	    logMessage(LOG_ERROR, "Failed to open shader file\n");
	    vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	    goto clean;
	}
	if (fseek(shaderFile, 0, SEEK_END) != 0) {
	    logMessage(LOG_ERROR, "Failed to seek shader file\n");
	    fclose(shaderFile);
	    goto clean;
	}
	long int pos = ftell(shaderFile);
	if (pos == -1L) {
	    logMessage(LOG_ERROR, "Failed to get shader file size\n");
	    fclose(shaderFile);
	    vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	    goto clean;
	}
	pipeline->spirvSize = (size_t)pos;
	pipeline->spirvBuffer = malloc(pipeline->spirvSize);
	if (fseek(shaderFile, 0, SEEK_SET) != 0) {
	    logMessage(LOG_ERROR, "Failed to seek shader file\n");
	    fclose(shaderFile);
	    goto clean;
	}
	const size_t retCode = fread(pipeline->spirvBuffer,
				     sizeof(*pipeline->spirvBuffer),
				     pipeline->spirvSize / sizeof(*pipeline->spirvBuffer),
				     shaderFile);
	if (retCode != (size_t)pipeline->spirvSize / sizeof(*pipeline->spirvBuffer)) {
	    logMessage(LOG_ERROR, "Failed to read shader file\n");
	    fclose(shaderFile);
	    vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	    goto clean;
	}
	logMessage(LOG_INFO, "Shader file read successfully\n");
	fclose(shaderFile);

	const VkShaderModuleCreateInfo shaderModuleInfo = {
	    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	    .pNext = NULL,
	    .flags = 0,
	    .codeSize = pipeline->spirvSize,
	    .pCode = pipeline->spirvBuffer
	};
	vulkan->result = vkCreateShaderModule(vulkan->device,&shaderModuleInfo, NULL, &vulkan->shaderModule);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to create shader module\n");
	    goto clean;
	}
	vulkan->result = VK_SUCCESS;
 clean:
	return (vulkan->result != VK_SUCCESS) ? false : true;
    }
#endif

#endif
