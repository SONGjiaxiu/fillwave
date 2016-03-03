/*
 * BuilderModelManual.cpp
 *
 *  Created on: Apr 27, 2015
 *      Author: filip
 */

#include <fillwave/models/builders/BuilderModelManual.h>

FLOGINIT("BuilderModelManual", FERROR | FFATAL)

namespace fillwave {
namespace framework {

/*! \class BuilderModelManual
 * \brief BuilderModel which builds the model from the asset file but uses external textures and meterial.
 */

BuilderModelManual::BuilderModelManual(
		Engine* engine,
		std::string modelPath,
		pProgram program,
		pTexture2D diffuseMap,
		pTexture2D normalMap,
		pTexture2D specularMap,
		Material material) :
			BuilderModel(engine, modelPath, program),
			mDiffuseMap(diffuseMap),
			mNormalMap(normalMap),
			mSpecularMap(specularMap),
			mMaterial(material) {

}

BuilderModelManual& BuilderModelManual::setDiffuseMapTexture(
		pTexture2D texture) {
	mDiffuseMap = texture;
	return (*this);
}

BuilderModelManual& BuilderModelManual::setNormalMapTexture(
		pTexture2D texture) {
	mNormalMap = texture;
	return (*this);
}

BuilderModelManual& BuilderModelManual::setSpecularMapTexture(
		pTexture2D texture) {
	mSpecularMap = texture;
	return (*this);
}

BuilderModelManual& BuilderModelManual::setMaterial(Material material) {
	mMaterial = material;
	return (*this);
}

pModel BuilderModelManual::build() {
	return buildModel(mEngine, mProgram, mShapePath, mDiffuseMap, mNormalMap,
		mSpecularMap, mMaterial);
}

} /* framework */
} /* fillwave */