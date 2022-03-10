#include <glew.h>
#include <SDL.h>
#include "TrackBall.h"
#include "VMath.h"
#include "MMath.h"

using namespace MATH;


TrackBall::TrackBall() {
	setWindowDimensions();
	mouseDown = false;
}

TrackBall::~TrackBall() {


}

void TrackBall::setWindowDimensions() {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));
}

void TrackBall::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		onLeftMouseDown(sdlEvent.button.x, sdlEvent.button.y);
	}
	else if (sdlEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP) {
		onLeftMouseUp(sdlEvent.button.x, sdlEvent.button.y);
	}
	else if (sdlEvent.type == SDL_MOUSEMOTION && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		onMouseMove(sdlEvent.button.x, sdlEvent.button.y);
	}
	else if (sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		setWindowDimensions();
	}
}

void TrackBall::onLeftMouseDown(int x, int y) {
	beginV = getMouseVector(x, y);
	mouseDown = true;

}

void TrackBall::onLeftMouseUp(int x, int y) {
	mouseDown = false;
}

void TrackBall::onMouseMove(int x, int y) {
	if (mouseDown == false) return;
	endV = getMouseVector(x, y);
	float cosAngle = VMath::dot(beginV, endV);
	if (cosAngle <= VERY_SMALL) {
		printf("%f\n", cosAngle);
	}
	float angle = acos(cosAngle) * 180.0f / M_PI;
	Vec3 rotAxis = VMath::cross(beginV, endV);

	mouseRotationMatrix = MMath::rotate(angle, rotAxis) * mouseRotationMatrix;

	beginV = endV;
}

 const Vec3 TrackBall::getMouseVector(int x, int y) {
	 Vec3 mousePosition(static_cast<float>(x), static_cast<float>(y), 0.0f);
	 Vec3 v = invNDC * mousePosition;
	 float xSquared = v.x * v.x;
	 float ySquared = v.y * v.y;

	 if (xSquared + ySquared <= 0.5f) {
		 v.z = sqrt(1.0 - (xSquared + ySquared));
	 }
	 else {
		 v.z = 0.5 / sqrt(xSquared + ySquared);
		 v = VMath::normalize(v);
	 }
	 return v;
 }
