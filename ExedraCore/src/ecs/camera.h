#pragma once
#include "transform.h"
#include "componentsystem.h"
#include "src/resources/rendertexture.h"
#include "src/gui/renderview.h"
#include <entt/entt.hpp>
#include <glad/glad.h>
#include "src/graphics/camera.h"

namespace exedra {
	namespace ecs {
		struct CameraComponent {
			CameraComponent()
				: fov(60), nearClipPlane(0.1), farClipPlane(100.0f) {}

			CameraComponent(float _fov, float _nearClipPlane, float _farClipPlane) {
				fov = _fov;
				nearClipPlane = _nearClipPlane;
				farClipPlane = _farClipPlane;
				targetTexture.Init(1, 1);
				clearColor = glm::vec4(0);

				SetClearColor({ 21.0 / 255, 32.0 / 255, 43.0 / 255, 1 });

				gui::RenderView* renderWindow = new gui::RenderView();
				renderWindow->Init(targetTexture);
				graphics::Window::current->GetImGui().AddWindow(renderWindow);

				//LOG_CORE_DEBUG("Create Camera");
			}

			glm::mat4 GetViewMatrix(const TransformComponent& _transform) const {
				glm::vec3 lookDirection = glm::toMat4(_transform.rotation) * glm::vec4(0, 0, 1, 0);
				return glm::lookAt(_transform.position, lookDirection + _transform.position, { 0, 1, 0 });
			}

			glm::mat4 GetProjectionMatrix() const {
				//float screenRatio = (float)targetTexture.GetWidth() / targetTexture.GetHeight();
				float screenRatio = graphics::Camera::current->screenRatio;
				return glm::perspective(glm::radians(fov), screenRatio, nearClipPlane, farClipPlane);
			}

			glm::mat4 GetViewProjectionMatrix(const TransformComponent& _transform) const {
				return GetProjectionMatrix() * GetViewMatrix(_transform);
			}

			void Clear() const {
				glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			void SetClearColor(glm::vec4 _rgba) {
				clearColor = _rgba;
			}

			void Bind() const {
				targetTexture.BindFB();
			}

			void Unbind() const {
				targetTexture.UnbindFB();
			}

		private:
			float fov;
			float nearClipPlane;
			float farClipPlane;

			glm::vec4 clearColor;
			res::RenderTexture targetTexture;
		};

		class CameraSystem : public ComponentSystem {
		public:
			void Start() override;
			void Update(entt::registry& _registry) override;
		};
	}
}