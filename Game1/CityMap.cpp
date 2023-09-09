#include "stdafx.h"
#include "CityMap.h"

CityMap* CityMap::Create(string name)
{
	CityMap* temp = new CityMap();
	temp->name = name;
	temp->LoadFile("CityMap.xml");
	temp->type = ObType::Actor;
	return temp;
}

CityMap::CityMap()
{
	cam = Camera::Create("CityCam");
	cam->LoadFile("CityCam.xml");
	ResizeScreen();

	grid = Grid::Create();

}

CityMap::~CityMap()
{
}

void CityMap::Init()
{
}

void CityMap::Release()
{
	cam->SaveFile("CityCam.xml");
	SaveFile("CityMap.xml");
}

void CityMap::Update()
{
	cam->Update();
	grid->Update();
	Actor::Update();
}

void CityMap::LateUpdate()
{
}

void CityMap::Render()
{
	cam->Set();
	Actor::Render();
	grid->Render();

}

void CityMap::ResizeScreen()
{
	cam->viewport.x = 0.0f;
	cam->viewport.y = 0.0f;
	cam->viewport.width = App.GetWidth();
	cam->viewport.height = App.GetHeight();
	cam->width = App.GetWidth();
	cam->height = App.GetHeight();
}

void CityMap::Hierarchy()
{
	//grid->RenderHierarchy();
	cam->RenderHierarchy();
	RenderHierarchy();
}
