/******************************************************************************
* Spine Runtimes Software License v2.5
*
* Copyright (c) 2013-2016, Esoteric Software
* All rights reserved.
*
* You are granted a perpetual, non-exclusive, non-sublicensable, and
* non-transferable license to use, install, execute, and perform the Spine
* Runtimes software and derivative works solely for personal or internal
* use. Without the written permission of Esoteric Software (see Section 2 of
* the Spine Software License Agreement), you may not (a) modify, translate,
* adapt, or develop new applications using the Spine Runtimes or otherwise
* create derivative works or improvements of the Spine Runtimes or (b) remove,
* delete, alter, or obscure any trademarks or any copyright, trademark, patent,
* or other intellectual property or proprietary rights notices on or in the
* Software, including any copy thereof. Redistributions in binary or source
* form must include this license and terms.
*
* THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS INTERRUPTION, OR LOSS OF
* USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "spine-sfml.h"

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

#define VERTEX_X1 0
#define VERTEX_Y1 1
#define VERTEX_BOTH1 0
#define VERTEX_X2 2
#define VERTEX_Y2 3
#define VERTEX_BOTH2 1
#define VERTEX_X3 4
#define VERTEX_Y3 5
#define VERTEX_BOTH3 2
#define VERTEX_X4 6
#define VERTEX_Y4 7
#define VERTEX_BOTH4 3

using namespace sf;

//void _AtlasPage_createTexture(AtlasPage* self, const char* path){
//	Texture* texture = new Texture();
//	if (!texture->loadFromFile(path)) return;
//
//	if (self->magFilter == SP_ATLAS_LINEAR) texture->setSmooth(true);
//	if (self->uWrap == SP_ATLAS_REPEAT && self->vWrap == SP_ATLAS_REPEAT) texture->setRepeated(true);
//
//	self->rendererObject = texture;
//	Vector2u size = texture->getSize();
//	self->width = size.x;
//	self->height = size.y;
//}

//void _AtlasPage_disposeTexture(AtlasPage* self){
//	delete (Texture*)self->rendererObject;
//}

//char* _Util_readFile(const char* path, int* length){
//	return _readFile(path, length);
//}

/**/

namespace spine {

	SkeletonDrawable::SkeletonDrawable(SkeletonData* skeletonData, AnimationStateData* stateData)
		: timeScale(1)
		, vertexArray(new VertexArray(Triangles, skeletonData->bones.size() * 4))
		, worldVertices(0)
	{
		//Bone_setYDown(true);
		worldVertices = new float[SPINE_MESH_VERTEX_COUNT_MAX];
		skeleton = new Skeleton(*skeletonData);

		ownsAnimationStateData = stateData == 0;
		if (ownsAnimationStateData) stateData = new AnimationStateData(*skeletonData);

		state = new AnimationState(*stateData);
	}

	SkeletonDrawable::~SkeletonDrawable() {
		delete vertexArray;
		delete []worldVertices;
		//if (ownsAnimationStateData) AnimationStateData_dispose(state->data);
		delete state;
		delete skeleton;
	}

	void SkeletonDrawable::update(float deltaTime) {
		skeleton->update(deltaTime);
		state->update(deltaTime*timeScale);
		state->apply(*skeleton);
		skeleton->updateWorldTransform();
	}

	void SkeletonDrawable::draw(RenderTarget& target, RenderStates states) const {
		vertexArray->clear();

		sf::Vertex vertices[4];
		sf::Vertex vertex;
		for (int i = 0; i < skeleton->slots.size(); ++i) {
			Slot* slot = skeleton->drawOrder[i];
			const Attachment* attachment = slot->getAttachment();
			if (!attachment) continue;

			sf::BlendMode blend;
			switch (slot->data.blendMode) {
			case spine::BlendMode::Additive:
				blend = BlendAdd;
				break;
			case spine::BlendMode::Multiply:
				blend = BlendMultiply;
				break;
			case spine::BlendMode::Screen: // Unsupported, fall through.
			default:
				blend = BlendAlpha;
			}
			if (states.blendMode != blend) {
				target.draw(*vertexArray, states);
				vertexArray->clear();
				states.blendMode = blend;
			}

			Texture* texture = 0;
			if (attachment->type == spine::Attachment::Type::Region) {
				RegionAttachment* regionAttachment = (RegionAttachment*)attachment;
				texture = (Texture*)((spine::Atlas::Region*)regionAttachment->rendererObject)->page.rendererObject;
				regionAttachment->computeWorldVertices(slot->bone, worldVertices);

				Uint8 r = static_cast<Uint8>(skeleton->color.r * slot->color.r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->color.g * slot->color.g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->color.b * slot->color.b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->color.a * slot->color.a * 255);

				Vector2u size = texture->getSize();
				vertices[0].color.r = r;
				vertices[0].color.g = g;
				vertices[0].color.b = b;
				vertices[0].color.a = a;
				vertices[0].position.x = worldVertices[VERTEX_X1];
				vertices[0].position.y = worldVertices[VERTEX_Y1];
				vertices[0].texCoords.x = regionAttachment->uvs[VERTEX_BOTH1].x * size.x;
				vertices[0].texCoords.y = regionAttachment->uvs[VERTEX_BOTH1].y * size.y;

				vertices[1].color.r = r;
				vertices[1].color.g = g;
				vertices[1].color.b = b;
				vertices[1].color.a = a;
				vertices[1].position.x = worldVertices[VERTEX_X2];
				vertices[1].position.y = worldVertices[VERTEX_Y2];
				vertices[1].texCoords.x = regionAttachment->uvs[VERTEX_BOTH2].x * size.x;
				vertices[1].texCoords.y = regionAttachment->uvs[VERTEX_BOTH2].y * size.y;

				vertices[2].color.r = r;
				vertices[2].color.g = g;
				vertices[2].color.b = b;
				vertices[2].color.a = a;
				vertices[2].position.x = worldVertices[VERTEX_X3];
				vertices[2].position.y = worldVertices[VERTEX_Y3];
				vertices[2].texCoords.x = regionAttachment->uvs[VERTEX_BOTH3].x * size.x;
				vertices[2].texCoords.y = regionAttachment->uvs[VERTEX_BOTH3].y * size.y;

				vertices[3].color.r = r;
				vertices[3].color.g = g;
				vertices[3].color.b = b;
				vertices[3].color.a = a;
				vertices[3].position.x = worldVertices[VERTEX_X4];
				vertices[3].position.y = worldVertices[VERTEX_Y4];
				vertices[3].texCoords.x = regionAttachment->uvs[VERTEX_BOTH4].x * size.x;
				vertices[3].texCoords.y = regionAttachment->uvs[VERTEX_BOTH4].y * size.y;

				vertexArray->append(vertices[0]);
				vertexArray->append(vertices[1]);
				vertexArray->append(vertices[2]);
				vertexArray->append(vertices[0]);
				vertexArray->append(vertices[2]);
				vertexArray->append(vertices[3]);

			}
			else if (attachment->type == spine::Attachment::Type::Mesh) {
				MeshAttachment* mesh = (MeshAttachment*)attachment;
				if (mesh->worldVerticesCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((spine::Atlas::Region*)mesh->rendererObject)->page.rendererObject;
				mesh->computeWorldVertices(*slot, worldVertices);

				Uint8 r = static_cast<Uint8>(skeleton->color.r * slot->color.r * 255);
				Uint8 g = static_cast<Uint8>(skeleton->color.g * slot->color.g * 255);
				Uint8 b = static_cast<Uint8>(skeleton->color.b * slot->color.b * 255);
				Uint8 a = static_cast<Uint8>(skeleton->color.a * slot->color.a * 255);
				vertex.color.r = r;
				vertex.color.g = g;
				vertex.color.b = b;
				vertex.color.a = a;

				Vector2u size = texture->getSize();
				for (int i = 0; i < mesh->triangles.size(); ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoords.x = mesh->uvs[index / 2].x * size.x;
					vertex.texCoords.y = mesh->uvs[index / 2].y * size.y;
					vertexArray->append(vertex);
				}

			}

			if (texture) {
				// SMFL doesn't handle batching for us, so we'll just force a single texture per skeleton.
				states.texture = texture;
			}
		}
		target.draw(*vertexArray, states);
	}

} /* namespace spine */