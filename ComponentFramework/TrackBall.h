#ifndef  TRACKBALL_H
#define	TRACKBALL_H
#include "MMath.h"

union SDL_Event;
using namespace MATH;

class TrackBall {
private:
	bool mouseDown;
	Matrix4 mouseRotationMatrix;
	Matrix4 invNDC;
	Vec3 beginV, endV;

public:
	TrackBall();
	~TrackBall();

	TrackBall(const TrackBall&) = delete;
	TrackBall( TrackBall&&) = delete;
	TrackBall& operator=(const TrackBall&&) = delete;
	TrackBall& operator=( TrackBall&&) = delete;

	const Matrix4 getMatrix4() const { return mouseRotationMatrix; }
	const Matrix3 getMatrix3() const { return Matrix3(mouseRotationMatrix); }

	void HandleEvents(const SDL_Event& sdlEvent);

private:
	const Vec3 getMouseVector(int x, int y);
	void setWindowDimensions();
	void onLeftMouseDown(int x, int y);
	void onLeftMouseUp(int x, int y);
	void onMouseMove(int x, int y);
};

#endif // ! TRACKBALL_H
