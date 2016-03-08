#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");
	//m_pMeshMngr->

	fDuration = 1.0f;
	vertices = std::vector<vector3>();
	//these points 
	vertices.push_back(vector3(-4.0f, -2.0f, 5.0f));
	vertices.push_back(vector3(1.0f, -2.0f, 5.0f));
	vertices.push_back(vector3(-3.0f, -1.0f, 3.0f));
	vertices.push_back(vector3(2.0f, -1.0f, 3.0f));
	vertices.push_back(vector3(-2.0f, 0.0f, 0.0f));
	vertices.push_back(vector3(3.0f, 0.0f, 0.0f));
	vertices.push_back(vector3(-1.0f, 1.0f, -3.0f));
	vertices.push_back(vector3(4.0f, 1.0f, -3.0f));
	vertices.push_back(vector3(0.0f, 2.0f, -5.0f));
	vertices.push_back(vector3(5.0f, 2.0f, -5.0f));
	vertices.push_back(vector3(1.0f, 3.0f, -5.0f));

	sphereMatrices = std::vector<matrix4>();
	for (int c = 0; c < vertices.size(); c++) {
		sphereMatrices.push_back(glm::translate(vertices[c]));
	}
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//get the current number of transitions completed, where 1 represents a single transition
	float transitions = fRunTime / fDuration;
	//if it's greater than the number of vertices, bring it back within range
	if (transitions > vertices.size())
		transitions -= std::floorf(transitions / (vertices.size()))*(vertices.size());
	//get the vertex to lerp from
	int first = static_cast<int>(std::floorf(transitions));
	//and the vertex to lerp to - setting it to 0 if it's outside the range of the list
	int second = (transitions > vertices.size() - 1) ? 0 : static_cast<int>(std::ceilf(transitions));

	//subtracting first from transition gets a value between 0 and 1 representing the position within the current transition
	m_pMeshMngr->SetModelMatrix(glm::translate(glm::lerp(vertices[first], vertices[second], transitions - first)), "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	for (int c = 0; c < sphereMatrices.size(); c++) {
		m_pMeshMngr->AddSphereToQueue(sphereMatrices[c]*glm::scale(.3f,.3f,.3f));
	}

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}