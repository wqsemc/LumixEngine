local dof_shader = nil
local dof_blur_shader = nil

function postprocess(env, phase, input, gbuffer0, gbuffer1, gbuffer_depth, shadowmap)
	if not enabled then return input end
	if phase ~= "post" then return input end
	env.beginBlock("dof")
	if dof_shader == nil then
		dof_shader = env.preloadShader("pipelines/dof.shd")
		dof_blur_shader = env.preloadShader("pipelines/dof_blur.shd")
	end

	local tmp_rb = env.createRenderbuffer(1, 1, true, "rgba16f", "dof_tmp")
	
	env.setRenderTargets(tmp_rb)
	env.drawArray(0, 4, dof_blur_shader, 
		{ 
			input,
			gbuffer_depth
		},
		{},
		{ "HBLUR", "NEAR" },
		{ depth_test = false, blending = "alpha"}
	)

	env.setRenderTargets(input)
	env.drawArray(0, 4, dof_blur_shader, 
		{
			tmp_rb,
			gbuffer_depth
		},
		{},
		{ "NEAR" },
		{ depth_test = false, blending = "alpha"}
	)

	env.setRenderTargets(tmp_rb)
	env.drawArray(0, 4, dof_blur_shader, 
		{ 
			input,
			gbuffer_depth
		},
		{},
		{ "HBLUR", "FAR" },
		{ depth_test = false, blending = "alpha"}
	)

	env.setRenderTargets(input)
	env.drawArray(0, 4, dof_blur_shader, 
		{
			tmp_rb,
			gbuffer_depth
		},
		{},
		{ "FAR" },
		{ depth_test = false, blending = "alpha"}
	)


	env.setRenderTargets(tmp_rb)
	env.drawArray(0, 4, dof_shader, 
		{
			input,
			gbuffer_depth
		}
	)

	
	env.endBlock()
	return tmp_rb
end

function awake()
	if _G["postprocesses"] == nil then
		_G["postprocesses"] = {}
	end
	table.insert(_G["postprocesses"], postprocess)
end

function onDestroy()
	for i, v in ipairs(_G["postprocesses"]) do
		if v == postprocess then
			table.remove(_G["postprocesses"], i)
			break;
		end
	end
end