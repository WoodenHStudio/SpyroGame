#include "pch.h"
#include "ResourceManager.h"
#include "Renderer.h"

// Static private members
AD_ULONG ResourceManager::current_id = 0;
AD_ULONG ResourceManager::effect_id = 0;

AD_ULONG ResourceManager::AddSimpleModel(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe) {
	ADUtils::SHADER shader = { 0 };

	if (!wireframe)
	{
		strcpy_s(shader.vshader, "files\\shaders\\simple_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\simple_ps.hlsl");
	}
	else
	{
		strcpy_s(shader.vshader, "files\\shaders\\debug_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\debug_ps.hlsl");
	}

	shader.wireframe = wireframe;

	return InitializeSimpleModel(modelname, materials, position, scale, rotation, shader);
}

AD_ULONG ResourceManager::AddAnimatedModel(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, bool wireframe) {
	ADUtils::SHADER shader = { 0 };

	if (!wireframe)
	{
		strcpy_s(shader.vshader, "files\\shaders\\animated_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\animated_ps.hlsl");
	}
	else
	{
		strcpy_s(shader.vshader, "files\\shaders\\debug_vs.hlsl");
		strcpy_s(shader.pshader, "files\\shaders\\debug_ps.hlsl");
	}

	shader.wireframe = wireframe;

	return InitializeAnimatedModel(modelname, materials, animations, position, scale, rotation, shader);
}

AD_ULONG ResourceManager::AddRenderableCollider(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\collider_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\collider_ps.hlsl");

	shader.wireframe = false;

	std::string modelname = "files\\models\\Cube.mesh";


	return InitializeSimpleModel(modelname, std::string(), pos, scale, rotation, shader);
}

AD_ULONG ResourceManager::AddLight(ADResource::ADRenderer::Light& light)
{
	ADResource::ADRenderer::Light temp;
	memcpy(&temp, &light, sizeof(ADResource::ADRenderer::Light));

	// Generate id and add that shit
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = lights.size();
	light_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	lights.push_back(temp);

	return id;
}

void ResourceManager::AddSkybox(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	ADUtils::SHADER shader = { 0 };
	strcpy_s(shader.vshader, "files\\shaders\\skybox_vs.hlsl");
	strcpy_s(shader.pshader, "files\\shaders\\skybox_ps.hlsl");

	//ADUtils::LoadWobjectMesh(modelname.c_str(), skybox, ADResource::ADRenderer::PBRRenderer::GetRendererResources()->device, shader);
	ADUtils::LoadStaticMesh(modelname.c_str(), skybox, ADResource::ADRenderer::PBRRenderer::GetRendererResources()->device, shader, materials);

	skybox.position = position;
	skybox.scale = scale;
	skybox.rotation = rotation;
}

AD_ULONG ResourceManager::GenerateUniqueID()
{
	return ResourceManager::current_id++;
}

AD_ULONG ResourceManager::GenerateEffectID()
{
	return ResourceManager::effect_id++;
}

AD_ULONG ResourceManager::InitializeSimpleModel(std::string modelname, std::string materials, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader)
{
	SimpleStaticModel* temp = new SimpleStaticModel();

	temp->position = position;
	temp->scale = scale;
	temp->rotation = rotation;

	ADUtils::LoadStaticMesh(modelname.c_str(), *temp, ADResource::ADRenderer::PBRRenderer::GetRendererResources()->device, shader, materials);

	if (temp->albedo) {
		ADResource::ADRenderer::PBRRenderer::GetRendererResources()->context->GenerateMips(temp->albedo.Get());
	}

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = fbxmodels.size();
	fbxmodel_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	fbxmodels.push_back(temp);

	return id;
}

AD_ULONG ResourceManager::InitializeAnimatedModel(std::string modelname, std::string materials, std::vector<std::string> animations, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, ADUtils::SHADER& shader)
{
	SimpleAnimModel* temp = new SimpleAnimModel();
	
	temp->position = position;
	temp->scale = scale;
	temp->rotation = rotation;

	ADUtils::LoadAnimatedMesh(modelname.c_str(), *temp, animations, ADResource::ADRenderer::PBRRenderer::GetRendererResources()->device, shader, materials);

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = fbxmodels.size();
	fbxmodel_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	fbxmodels.push_back(temp);

	return id;
}

AD_ULONG ResourceManager::InitializeColliderModel(std::string modelname, ADPhysics::Collider* collider, ADUtils::SHADER& shader)
{
	if (modelname == "")
		return -1;

	SimpleStaticModel* temp = new SimpleStaticModel();

	temp->position = collider->Pos;

	if (collider->type == ADPhysics::ColliderType::Sphere)
	{
		ADPhysics::Sphere* sphere = static_cast<ADPhysics::Sphere*>(collider);
		temp->scale = XMFLOAT3(sphere->Radius, sphere->Radius, sphere->Radius);
		temp->rotation = XMFLOAT3(0, 0, 0);
	}
	else if (collider->type == ADPhysics::ColliderType::Aabb) 
	{
		ADPhysics::AABB* aabb = static_cast<ADPhysics::AABB*>(collider);
		temp->scale = aabb->HalfSize;
		temp->rotation = XMFLOAT3(0, 0, 0);
	}
	else if (collider->type == ADPhysics::ColliderType::Obb) 
	{
		ADPhysics::OBB* obb = static_cast<ADPhysics::OBB*>(collider);
		temp->scale = obb->HalfSize;
		temp->rotation = XMFLOAT3(0, 0, 0);
	}

	ADUtils::LoadStaticMesh(modelname.c_str(), *temp, ADResource::ADRenderer::PBRRenderer::GetRendererResources()->device, shader);

	// grab id and add stuff
	AD_ULONG id = GenerateUniqueID();
	unsigned int index = collider_map.size();
	collider_map.insert(std::pair<AD_ULONG, unsigned int>(id, index));
	colliders.push_back(temp);

	return id;
}

void ResourceManager::ConfigureUnifiedBuffers(ComPtr<ID3D11Device1> device)
{
	// Setup Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA vdata;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&vdata, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(Vertex) * pbrVertexData.size();
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdata.pSysMem = pbrVertexData.data();

	HRESULT result = device->CreateBuffer(&bufferDesc, &vdata, &vertexBuffer);
	assert(!FAILED(result));

	// Index buffer
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * pbrIndxData.size();

	D3D11_SUBRESOURCE_DATA idata;
	ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
	idata.pSysMem = pbrIndxData.data();
	result = device->CreateBuffer(&bufferDesc, &idata, &indexBuffer);
	assert(!FAILED(result));
}

Microsoft::WRL::ComPtr<ID3D11Buffer> ResourceManager::GetVertexBuffer()
{
	return vertexBuffer;
}


Microsoft::WRL::ComPtr<ID3D11Buffer> ResourceManager::GetIndexBuffer()
{
	return indexBuffer;
}

void ResourceManager::AddModelToRenderQueue(ADResource::ADGameplay::GameObject* obj)
{
	render_queue.push(obj);
}

bool ResourceManager::RenderQueueEmpty()
{
	return render_queue.empty();
}

void ResourceManager::AddModelToColliderQueue(AD_ULONG key)
{
	collider_queue.push(key);
}

bool ResourceManager::ColliderQueueEmpty()
{
	return collider_queue.empty();
}

ADResource::ADGameplay::GameObject* ResourceManager::PopFromRenderQueue()
{
	ADResource::ADGameplay::GameObject* temp = render_queue.front();
	render_queue.pop();

	return temp;
}

AD_ULONG ResourceManager::PopFromColliderQueue() 
{
	AD_ULONG temp = collider_queue.front();
	collider_queue.pop();

	return temp;
}

int ResourceManager::AddGameObject(ADResource::ADGameplay::GameObject* OBJ)
{
	int temp = gameobjects.size();

	gameobjects.push_back(OBJ);

	return temp;
}

void ResourceManager::RemoveGameObject(ADResource::ADGameplay::GameObject* OBJ)
{
	int temp = gameobjects.size();

	gameobjects.erase(std::remove(gameobjects.begin(), gameobjects.end(), OBJ));

}

int ResourceManager::GetLightCount()
{
	return lights.size();
}

char* ResourceManager::GetLightDataPtr()
{
	return (char*)lights.data();
}

ADResource::ADRenderer::SimpleStaticModel* ResourceManager::GetSkybox()
{
	return &skybox;
}

ADResource::ADRenderer::Light* ResourceManager::GetLightPtr()
{
	return lights.data();
}

int ResourceManager::GetGameObjectCount()
{
	return gameobjects.size();
}

ADResource::ADGameplay::GameObject** ResourceManager::GetGameObjectPtr()
{
	return gameobjects.data();
}

ADResource::ADRenderer::SimpleModel** ResourceManager::GetSimpleModelPtrFromMeshId(AD_ULONG mesh_id)
{
	SimpleModel** temp = nullptr;
	std::unordered_map<AD_ULONG, unsigned int>::const_iterator iter = fbxmodel_map.find(mesh_id);

	if (iter != fbxmodel_map.end())
	{
		// Found
		temp = &fbxmodels[iter->second];
	}

	return temp;
}

ADResource::ADRenderer::SimpleModel** ResourceManager::GetColliderPtrFromMeshId(AD_ULONG mesh_id) 
{
	SimpleModel** temp = nullptr;
	std::unordered_map<AD_ULONG, unsigned int>::const_iterator iter = collider_map.find(mesh_id);

	if (iter != collider_map.end()) 
	{
		temp = &colliders[iter->second];
	}

	return temp;
}