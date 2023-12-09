#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

void vkhelper2_renderpass_config_offscreen(
	Vkhelper2RenderpassConfig *conf
) {
	VkAttachmentDescription attach_color = {
		.format = VK_FORMAT_B8G8R8A8_UNORM,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
	conf->descs[0] = attach_color;

	conf->color_ref = (VkAttachmentReference) {
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	conf->subpass = (VkSubpassDescription) {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &conf->color_ref,
	};

	conf->info = (VkRenderPassCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.flags = 0,
		.attachmentCount = 1,
		.pAttachments = conf->descs,
		.subpassCount = 1,
		.pSubpasses = &conf->subpass,
	};
}

void vkhelper2_renderpass_config(
	Vkhelper2RenderpassConfig *conf,
	VkFormat format,
	VkFormat depth_format
) {
	VkAttachmentDescription attach_color = {
		.format = format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};
	VkAttachmentDescription attach_depth = {
		.format = depth_format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};
	conf->descs[0] = attach_color;
	conf->descs[1] = attach_depth;

	conf->color_ref = (VkAttachmentReference) {
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};
	conf->depth_ref = (VkAttachmentReference) {
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};

	conf->subpass = (VkSubpassDescription) {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &conf->color_ref,
		.pDepthStencilAttachment = &conf->depth_ref,
	};

	conf->info = (VkRenderPassCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.flags = 0,
		.attachmentCount = 2,
		.pAttachments = conf->descs,
		.subpassCount = 1,
		.pSubpasses = &conf->subpass,
		.dependencyCount = 0,
		.pDependencies = NULL,
	};
}

void vkhelper2_renderpass_build(
	VkRenderPass *result,
	Vkhelper2RenderpassConfig *conf,
	VkDevice device) {
	assert(0 == vkCreateRenderPass(device, &conf->info, NULL, result));
}

void vkhelper2_renderpass_begin(VkCommandBuffer cbuf,
	VkRenderPass rp,
	VkFramebuffer fb,
	uint32_t width, uint32_t height
) {
	VkRenderPassBeginInfo rbegin = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = rp,
		.framebuffer = fb,
		.renderArea.extent.width = width,
		.renderArea.extent.height = height,
	};
	vkCmdBeginRenderPass(cbuf, &rbegin, VK_SUBPASS_CONTENTS_INLINE);
}

void vkhelper2_renderpass_begin_clear(VkCommandBuffer cbuf,
	VkRenderPass rp, VkFramebuffer fb,
	uint32_t width, uint32_t height
) {
	static const VkClearValue clear_color = {
		.color.float32 = {0.0f, 0.0f, 0.0f, 0.0f},
	};
	static const VkClearValue clear_depthstencil = {
		.depthStencil.depth = 1.0f,
		.depthStencil.stencil = 0,
	};
	VkClearValue clears[2] = {clear_color, clear_depthstencil};
	VkRenderPassBeginInfo rbegin = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = rp,
		.framebuffer = fb,
		.renderArea.extent.width = width,
		.renderArea.extent.height = height,
		.clearValueCount = 2,
		.pClearValues = clears,
	};
	vkCmdBeginRenderPass(cbuf, &rbegin, VK_SUBPASS_CONTENTS_INLINE);
}
