#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

VkSampler vkhelper2_sampler(VkDevice device) {
	VkSampler sampler;
	VkSamplerCreateInfo sampler_info = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
	};
	assert(0 == vkCreateSampler(device, &sampler_info,
		NULL, &sampler));
	return sampler;
}
