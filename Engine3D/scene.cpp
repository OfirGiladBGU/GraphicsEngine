#include "scene.h"
#include "glad/include/glad/glad.h"
#include <iostream>

static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f ", mat[j][i]);
		}
		printf("\n");
	}
}

Scene::Scene()
{
	glLineWidth(5);
	
	cameras.push_back(new Camera(60.0f,1.0,0.1f,100.0f));		
	pickedShape = -1;
	//depth = 0;
	cameraIndx = 0;
	xold = 0;
	yold = 0;
	isActive = false;
}

Scene::Scene(float angle, float relationWH, float near1, float far1)
{
	//glLineWidth(5);

	cameras.push_back(new Camera(angle, relationWH, near1, far1));
	pickedShape = -1;
	//depth = 0;
	cameraIndx = 0;
	xold = 0;
	yold = 0;
	isActive = false;

	// Set the camera parameters
	camera_angle = angle;
	camera_near = near1;
	camera_far = far1;
}

void Scene::AddShapeFromFile(const std::string& fileName,int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(fileName, mode));
}

void Scene::AddShape(int type, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(type, mode));
}

void Scene::AddShapeCopy(int indx,int parent,unsigned int mode)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(*shapes[indx], mode));
}

void Scene::AddShader(const std::string& fileName)
{
	shaders.push_back(new Shader(fileName));
}

void Scene::AddTexture(const std::string& textureFileName, bool for2D)
{
	textures.push_back(new Texture(textureFileName));
}

void Scene::AddTexture(int width,int height, unsigned char *data)
{
	textures.push_back(new Texture(width, height, data));
}

void Scene::AddCamera(glm::vec3& pos, float fov, float relationWH, float zNear, float zFar)
{
	cameras.push_back(new Camera(fov, relationWH, zNear, zFar));
	cameras.back()->MyTranslate(pos, 0);
}

void Scene::Draw(int shaderIndx, int cameraIndx, int buffer, bool toClear, bool debugMode, int viewportIndex)
{
	glEnable(GL_DEPTH_TEST);
	glm::mat4 Normal = MakeTrans();
	glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection() * glm::inverse(cameras[cameraIndx]->MakeTrans());
	int p = pickedShape;

	// Clear the screen
	if (toClear)
	{	
		if (shaderIndx == 0) // Picking Shader
		{
			Clear(0, 0, 0, 0);  // Black Screen
		}
		else // Other Shaders (Basic Shader)
		{
			Clear(1, 1, 1, 1);  // White Screen
		}
	}

	// Choose 1 of the 4 Viewport
	if (viewportIndex == 0) // Top Left
	{
		glViewport(0, height / 2, width / 2, height / 2);  
	}
	else if (viewportIndex == 1) // Top Right
	{
		glViewport(width / 2, height / 2, width / 2, height / 2);
	}
	else if (viewportIndex == 2) // Bottom Left
	{
		glViewport(0, 0, width / 2, height / 2);
	}
	else if (viewportIndex == 3) // Bottom Right
	{
		glViewport(width / 2, 0, width / 2, height / 2);
	}

	// Draw the shapes
	for (unsigned int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i]->Is2Render())
		{
			glm::mat4 Model = Normal * shapes[i]->MakeTrans();
			
			if (shaderIndx == 0) // Picking Shader
			{
				pickedShape = i;
				Update(MVP, Model, 0);
				shapes[i]->Draw(shaders, textures, true);
			}
			else // Other Shaders (Basic Shader)
			{
				Update(MVP, Model, shapes[i]->GetShader());
				shapes[i]->Draw(shaders, textures, false);
			}
		}
	}
	pickedShape = p;
}

void Scene::MoveCamera(int cameraIndx, int type, float amt)
{
	switch (type)
	{
		case xTranslate:
			cameras[cameraIndx]->MyTranslate(glm::vec3(amt, 0, 0), 0);
		break;
		case yTranslate:
			cameras[cameraIndx]->MyTranslate(glm::vec3(0, amt, 0), 0);
		break;
		case zTranslate:
			cameras[cameraIndx]->MyTranslate(glm::vec3(0, 0, amt), 0);
		break;
		case xRotate:
			cameras[cameraIndx]->MyRotate(amt, glm::vec3(1, 0, 0), 0);
		break;
		case yRotate:
			cameras[cameraIndx]->MyRotate(amt, glm::vec3(0, 1, 0), 0);
		break;
		case zRotate:
			cameras[cameraIndx]->MyRotate(amt, glm::vec3(0, 0, 1), 0);
		break;
		default:
			break;
	}
}

void Scene::ShapeTransformation(int type, float amt)
{
	if(glm::abs(amt)>1e-5)
	{
		switch (type)
		{
		case xTranslate:
			shapes[pickedShape]->MyTranslate(glm::vec3(amt,0,0),0);
		break;
		case yTranslate:
			shapes[pickedShape]->MyTranslate(glm::vec3(0,amt,0),0);
		break;
		case zTranslate:
			shapes[pickedShape]->MyTranslate(glm::vec3(0,0,amt),0);
		break;
		case xRotate:
			shapes[pickedShape]->MyRotate(amt,glm::vec3(1,0,0),0);
		break;
		case yRotate:
			shapes[pickedShape]->MyRotate(amt,glm::vec3(0,1,0),0);
		break;
		case zRotate:
			shapes[pickedShape]->MyRotate(amt,glm::vec3(0,0,1),0);
		break;
		default:
			break;
		}
	}

}

void Scene::Resize(int width, int height)
{
	cameras[0]->SetProjection(cameras[0]->GetAngle(), (float)width/height);
	glViewport(0, 0, width, height);
	//std::cout << "Camera Relation Width/Height: " << cameras[0]->GetRelationWH() << std::endl;

	// Set the width and height
	this->width = width;
	this->height = height;
}

float Scene::Picking(int x, int y)
{
	// TODO: Implement Picking
	x_picked = x;
	y_picked = this->height - y;
	return 0;
}

//return coordinates in global system for a tip of arm position is local system 
void Scene::MouseProccessing(int button)
{
	if (pickedShape == -1)
	{
		if (button == 1)
		{
			MyTranslate(glm::vec3(-xrel / 20.0f, 0, 0), 0);
			MyTranslate(glm::vec3(0, yrel / 20.0f, 0), 0);
			WhenTranslate();
		}
		else
		{
			MyRotate(xrel / 2.0f, glm::vec3(0, 1, 0), 0);
			MyRotate(yrel / 2.0f, glm::vec3(1, 0, 0), 0);
			WhenRotate();
		}
	}
	else
	{
		// TODO: Implement Z-Buffer
		if (button == 1)
		{
			// TODO: Use Z-Buffer to Implement Translate
			WhenTranslate();
		}
		else
		{
			// TODO: Use camera parameters to Implement Rotation according to current camera view
			WhenRotate();
		}
	}
}

void Scene::ZeroShapesTrans()
{
	for (unsigned int i = 0; i < shapes.size(); i++)
	{
		shapes[i]->ZeroTrans();
	}
}

//void Scene::ReadPixel()
//{
//	glReadPixels(1, 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
//}

void Scene::ReadPixelDepth()
{
	glReadPixels(x_picked, y_picked, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth_picked);
}

void Scene::ReadPixelColor()
{
	unsigned char color_picked[]{ 0, 0, 0, 0 };
	glReadPixels(x_picked, y_picked, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color_picked);
	int color_id = color_picked[0] | color_picked[1] << 8 | color_picked[2] << 16;
	pickedShape = color_id - 1;
}

void Scene::UpdatePosition(float xpos, float ypos)
{
	xrel = xold - xpos;
	yrel = yold - ypos;
	xold = xpos;
	yold = ypos;
}

void Scene::HideShape(int shpIndx)
{
	shapes[shpIndx]->Hide();
}

void Scene::UnhideShape(int shpIndx)
{
	shapes[shpIndx]->Unhide();
}

void Scene::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Scene::~Scene(void)
{
	for (Shape* shp : shapes)
	{
		delete shp;
	}
	for (Camera* cam : cameras)
	{
		delete cam;
	}
	for(Shader* sdr: shaders)
	{
		delete sdr;
	}
	for(Texture* tex: textures)
	{
		delete tex;
	}
}
