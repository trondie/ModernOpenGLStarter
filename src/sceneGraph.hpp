#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>
#include <cstdio>
#include <stdbool.h>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <fstream>
#include "../src/utilities/mesh.h"
#include "utilities/shape.h"
#include "utilities/glutils.h"
#include <string>

enum SceneNodeType {
	GEOMETRY, POINT_LIGHT, SPOT_LIGHT, NONE
};

class SceneNode {

protected:

	/**
	 * Relative position wrt to its parent
	 */ 
	glm::vec3 position;
	
	/**
	 * Location / layout
	 */ 
	int location;

	/**
	 * The nodetype for this node . 
	 */ 
	SceneNodeType nodeType;

		/**
	 * All children attached to this parent
	 */  
	std::vector<SceneNode*> children;
	
	/**
	 * The node's reference point
	 */ 
	glm::vec3 referencePoint;
	
public:

	/**
	 * Current transformation matrix (model matrix) 
	 * Note, it has to be derived wrt to this class' parents 
	 */ 
	glm::mat4 currentTransformationMatrix;

	SceneNode() : position(glm::vec3(0, 0, 0)), referencePoint(glm::vec3(0, 0, 0)) { 
		nodeType=NONE; 
	}

	const int generateTextureId(std::string texturePath) const {
		PNGImage textureImage = loadTexture(texturePath); 
		return generateTexture(textureImage); 
	}

	void addChild(SceneNode * sceneNode){
			sceneNode->referencePoint = this->position; 
			this->children.push_back(sceneNode);
	}
	void changeRelativePosition(glm::vec3 newPosition) {	
		this->position = newPosition; 
		for (int i = 0; i < this->children.size(); ++i){
			this->children[i]->referencePoint = this->position; 
		}
	}

	virtual int getLocation() const = 0; 
	
	std::vector<SceneNode*> & getChildren() { return this->children; }

	const glm::vec3 getPosition() const { return this->position; }
	const glm::vec3 getReferencePoint() const { return this->referencePoint; }
	const SceneNodeType getNodetype() const { return this->nodeType; }

};

class EmptyNode : public SceneNode {
	public:
		EmptyNode() : SceneNode() { this->nodeType = NONE; }
	virtual int getLocation() const { return -1; }
};

class GeometryNode : public SceneNode {

private: 

	/**
	 * Shape for the geometry
	 */ 
	Shape * shape; 

	/**
	 * The VAO id for this node
	 */ 
	int vertexArrayObjectID;

	/**
	 * Index count 
	 */ 
	unsigned int VAOIndexCount;

	/**
	 * Rotation vector
	 */ 
	glm::vec3 rotation;

	/**
	 * Scale vector
	 */ 
	glm::vec3 scale;
	
	/**
	 * Texture IDs for texture and coordinate VAO 
	 */ 
	int textureId, texureVaoId;

public:

	GeometryNode() : SceneNode() { this->nodeType = GEOMETRY; }
	
	GeometryNode(const int location, Shape * shape, const glm::vec3 position, std::string texturePath) : SceneNode()  {
		
		nodeType = GEOMETRY;
		this->location = location; 
		this->shape = shape; 
		this->vertexArrayObjectID = generateBuffer(*(shape->getMesh())); 
		this->VAOIndexCount = shape->getMesh()->indices.size();

		if (texturePath.length() > 0) {	
			this->textureId = this->generateTextureId(texturePath); 
			this->texureVaoId = generateTextureCoordinateVaoFromMesh(*shape->getMesh());
		}
		this->position = position; 

		// Default
		referencePoint = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
	}

	virtual int getLocation() const { return this->location; }

	const glm::vec3 getRotation() const { return this->rotation; }
	const glm::vec3 getScale() const { return this->scale; }

	const Shape * getShape() const { return this->shape; }
	const int getTextureId() const { return this->textureId; }
	const int getTexureVaoId() const { return this->texureVaoId; }
	const int getVaoIndexCount() const { return this->VAOIndexCount; }
	const unsigned int getVaoId() const { return this->vertexArrayObjectID; }
};

class PointLightNode : public SceneNode {
	public:
		int lightId;
		PointLightNode(const int lightId, glm::vec3 position) {
			this->nodeType = POINT_LIGHT;
			this->position = position; 
			this->lightId = lightId;
		}
		virtual int getLocation() const { return this->lightId; }
};

