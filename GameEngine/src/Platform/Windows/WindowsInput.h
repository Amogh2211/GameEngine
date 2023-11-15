#pragma once
#include "Engine/Core/Input.h"


namespace GameEngine {
	class WindowsInput : public Input {
	protected:

		virtual bool IsKeyPressedImpl(int keycode) override; // actual realization of the interface
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};
}
