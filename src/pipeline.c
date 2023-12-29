#include <vulkan/vulkan.h>

#include "../../ppath/build/ppath.h"
#include "../include/vkhelper2.h"

#define VKHELPER_PATH_LEN 1024

void vkhelper2(pipeline_config)(Vkhelper2(PipelineConfig) *vpc,
	uint32_t vbc, uint32_t vac, uint32_t sets) {
	vpc->desc = malloc((size_t)sets * sizeof(VkDescriptorSetLayout));
	vpc->plci = (VkPipelineLayoutCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = sets,
		.pushConstantRangeCount = 0,
	};

	vpc->via = malloc(sizeof(VkVertexInputAttributeDescription) * vac);
	vpc->vib = malloc(sizeof(VkVertexInputBindingDescription) * vbc);

	vpc->stages[0] = (VkPipelineShaderStageCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.pName = "main",
	};

	vpc->stages[1] = (VkPipelineShaderStageCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.pName = "main",
	};

	vpc->vis = (VkPipelineVertexInputStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = vbc,
		.pVertexBindingDescriptions = vpc->vib,
		.vertexAttributeDescriptionCount = vac,
		.pVertexAttributeDescriptions = vpc->via,
	};

	vpc->ias = (VkPipelineInputAssemblyStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	vpc->vs = (VkPipelineViewportStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.scissorCount = 1,
	};

	vpc->rast = (VkPipelineRasterizationStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
	};

	vpc->ms = (VkPipelineMultisampleStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	vpc->cba = (VkPipelineColorBlendAttachmentState) {
		.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.alphaBlendOp = VK_BLEND_OP_ADD,
		.blendEnable = VK_TRUE,
	};
	vpc->cbs = (VkPipelineColorBlendStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.pAttachments = &vpc->cba,
		.attachmentCount = 1,
		.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
	};

	vpc->dss = (VkPipelineDepthStencilStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.depthBoundsTestEnable = VK_FALSE,
		.back = {
			.failOp = VK_STENCIL_OP_KEEP,
			.passOp = VK_STENCIL_OP_KEEP,
			.compareOp = VK_COMPARE_OP_ALWAYS,
		},
		.stencilTestEnable = VK_FALSE,
		.front = {
			.failOp = VK_STENCIL_OP_KEEP,
			.passOp = VK_STENCIL_OP_KEEP,
			.compareOp = VK_COMPARE_OP_ALWAYS,
		},
	};

	vpc->dy[0] = VK_DYNAMIC_STATE_VIEWPORT;
	vpc->dy[1] = VK_DYNAMIC_STATE_SCISSOR;
	vpc->dys = (VkPipelineDynamicStateCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = 2,
		.pDynamicStates = vpc->dy,
	};

	vpc->pci = (VkGraphicsPipelineCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pStages = vpc->stages,
		.pVertexInputState = &vpc->vis,
		.pInputAssemblyState = &vpc->ias,
		.pViewportState = &vpc->vs,
		.pRasterizationState = &vpc->rast,
		.pMultisampleState = &vpc->ms,
		.pDepthStencilState = &vpc->dss,
		.pColorBlendState = &vpc->cbs,
		.pDynamicState = &vpc->dys,
	};
}

void vkhelper2(pipeline_simple_shader)(Vkhelper2(PipelineConfig) *vpc,
	VkDevice device, char *src, char *relative
) {
	char path[VKHELPER_PATH_LEN];
	char *new = NULL;
	snprintf(path, VKHELPER_PATH_LEN, "../%s_vert.spv", relative);
	com_6e5d_ppath_rel(&new, src, path);
	vpc->stages[0].module = vkhelper2(shader_module)(device, new);
	snprintf(path, VKHELPER_PATH_LEN, "../%s_frag.spv", relative);
	com_6e5d_ppath_rel(&new, src, path);
	vpc->stages[1].module = vkhelper2(shader_module)(device, new);
	free(new);
}

void vkhelper2(pipeline_simple_shader2)(Vkhelper2(PipelineConfig) *vpc,
	VkDevice device, char *src, char *vert, char *frag
) {
	char *new = NULL;
	char path[VKHELPER_PATH_LEN];
	snprintf(path, VKHELPER_PATH_LEN, "../%s", vert);
	com_6e5d_ppath_rel(&new, src, path);
	vpc->stages[0].module = vkhelper2(shader_module)(device, new);
	snprintf(path, VKHELPER_PATH_LEN, "../%s", frag);
	com_6e5d_ppath_rel(&new, src, path);
	vpc->stages[1].module = vkhelper2(shader_module)(device, new);
	free(new);
}

void vkhelper2(pipeline_build)(
	VkPipelineLayout *layout,
	VkPipeline *ppl,
	Vkhelper2(PipelineConfig) *vpc,
	VkRenderPass renderpass,
	VkDevice device,
	uint32_t subpass
) {
	// all pointers in config should be assigned here
	// because user call malloc then they gets overwritten
	vpc->plci.pSetLayouts = vpc->desc;
	assert(0 == vkCreatePipelineLayout(device, &vpc->plci, NULL, layout));
	vpc->pci.layout = *layout;
	vpc->pci.renderPass = renderpass;
	vpc->pci.subpass = subpass;
	assert(0 == vkCreateGraphicsPipelines(
		device, VK_NULL_HANDLE, 1, &vpc->pci, NULL, ppl));
}

void vkhelper2(pipeline_config_deinit)(
	Vkhelper2(PipelineConfig) *vpc, VkDevice device) {
	free(vpc->via);
	free(vpc->vib);
	free(vpc->desc);
	vkDestroyShaderModule(device, vpc->stages[0].module, NULL);
	vkDestroyShaderModule(device, vpc->stages[1].module, NULL);
}
