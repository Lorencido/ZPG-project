#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, Transformation* t, glm::vec3 col)
: model(m), shader(s), transform(t), color(col) {}

void DrawableObject::draw() {
    model->BindVAO();
    shader->setVec3("objectColor", color);
    shader->setUniformBool("isSkybox", false);
    shader->setUniformMatrix4fv("modelMatrix", glm::value_ptr(transform->getModelMatrix()));
    model->DrawModel();

}

void DrawableObject::update() const {
    transform->update();
}

void DrawableObject::setID(unsigned int newID) {
    id = newID;
}
