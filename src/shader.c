#include <vulkan/vulkan.h>

#include "../../wholefile/build/wholefile.h"
#include "../include/vkhelper2.h"

VkShaderModule vkhelper2(shader_module)(
	VkDevice device,
	char *path
) {
	uint32_t *data;
	size_t len = com_6e5d_wholefile_read(path, (uint8_t**)&data);
	if (len % 4 != 0) {
		printf("bad spv shader code!\n");
		abort();
	}
	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = len,
		.pCode = data,
	};
	VkShaderModule shaderModule;
	assert(0 == vkCreateShaderModule(
		device, &createInfo, NULL, &shaderModule));
	// printf("loaded: %s\n", path);
	free(data);
	return shaderModule;
}
