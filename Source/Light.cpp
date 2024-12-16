#include "Graphics/Graphics.h"
#include "Light.h"

Light::Light(LightType lightType) : lightType(lightType)
{
}

void Light::PushRenderContext(RenderContext& rc) const
{
	// “o˜^‚³‚ê‚Ä‚¢‚éŒõŒ¹‚Ìî•ñ‚ðÝ’è
	switch (lightType)
	{
	case	LightType::Directional:
	{
		rc.directionalLightData.direction.x = direction.x;
		rc.directionalLightData.direction.y = direction.y;
		rc.directionalLightData.direction.z = direction.z;
		rc.directionalLightData.direction.w = 0.0f;
		rc.directionalLightData.color = color;
		break;
	}
	case	LightType::Point:
	{
		if (rc.pointLightCount >= PointLightMax)
			break;
		rc.pointLightData[rc.pointLightCount].position.x = position.x;
		rc.pointLightData[rc.pointLightCount].position.y = position.y;
		rc.pointLightData[rc.pointLightCount].position.z = position.z;
		rc.pointLightData[rc.pointLightCount].position.w = 1.0f;
		rc.pointLightData[rc.pointLightCount].color = color;
		rc.pointLightData[rc.pointLightCount].range = range;
		++rc.pointLightCount;
		break;
	}
	}

}

void Light::DrawDebugGUI()
{
	static constexpr char* lightTypeName[] =
	{
		"Directional",
		"Point",
	};

	if (ImGui::TreeNode(lightTypeName[static_cast<int>(lightType)]))
	{
		switch (lightType)
		{
		case LightType::Directional:
			if (ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f))
			{
				float l = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
				direction.x /= l;
				direction.y /= l;
				direction.z /= l;
			}
			ImGui::ColorEdit3("color", &color.x);
			break;

		case LightType::Point:
		{
			ImGui::DragFloat3("position", &position.x);
			ImGui::ColorEdit3("color", &color.x);
			ImGui::DragFloat("range", &range, 0.1f, 0, FLT_MAX);
			break;
		}
		}
		ImGui::TreePop();
	}
}

void Light::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	switch (lightType)
	{
	case LightType::Directional:
		//•½sŒõŒ¹‚Í•\Ž¦‚µ‚È‚¢
		break;
	case LightType::Point:
		//“_ŒõŒ¹‚Í‘S•ûˆÊ‚ÉŒõ‚ð•úo‚·‚éŒõŒ¹‚È‚Ì‚Å‹…‘Ì‚ð•`‰æ‚·‚é
		//debugRenderer->DrawSphere(position, range, color);
		break;
	}
}