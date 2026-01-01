#pragma once
#include "Transformation.h"
#include "Transform.h"
#include <vector>
#include <memory>

class CompositeTransformation : public Transformation {
    std::vector<std::shared_ptr<Transformation>> transformations;

public:
    CompositeTransformation() = default;

    void add(const std::shared_ptr<Transformation>& t);
    void setTranslation(const glm::vec3&);

    void update() override;
};