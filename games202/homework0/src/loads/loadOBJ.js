
function loadOBJ(renderer, path, name) {

	const manager = new THREE.LoadingManager();
	manager.onProgress = function (item, loaded, total) {
		console.log(item, loaded, total);
	};

	function onProgress(xhr) {
		if (xhr.lengthComputable) {
			const percentComplete = xhr.loaded / xhr.total * 100;
			console.log('model ' + Math.round(percentComplete, 2) + '% downloaded');
		}
	}
	function onError() { }

	new THREE.MTLLoader(manager)
		.setPath(path)
		.load(name + '.mtl', function (materials) {
			materials.preload();
			new THREE.OBJLoader(manager)
				.setMaterials(materials)
				.setPath(path)
				.load(name + '.obj', function (object) {
					object.traverse(function (child) {
						if (child.isMesh) {
							let geo = child.geometry;
							let mat;
							if (Array.isArray(child.material)) mat = child.material[0];
							else mat = child.material;

							var indices = Array.from({ length: geo.attributes.position.count }, (v, k) => k);
							let mesh = new Mesh({ name: 'aVertexPosition', array: geo.attributes.position.array },
								{ name: 'aNormalPosition', array: geo.attributes.normal.array },
								{ name: 'aTextureCoord', array: geo.attributes.uv.array },
								indices);

							let colorMap = null;
							if (mat.map != null) colorMap = new Texture(renderer.gl, mat.map.image);
							// MARK: You can change the myMaterial object to your own Material instance

							let textureSample = 0;
							let myMaterial;
							let light;
							if (Array.isArray(renderer.lights)) 
							{
								light = renderer.lights[0];
							}
							else 
							{
								light = renderer.lights;
							}

							if (colorMap != null) {
								textureSample = 1;
								myMaterial = new Material({
									'uSampler': { type: 'texture', value: colorMap },
									'uTextureSample': { type: '1i', value: textureSample },
									'uKd': { type: '3fv', value: mat.color.toArray() },
									'uks':{type:'3fv', value: light.entity.mat.color },
									'uLightIntensity':{type:'1f', value: 10.0 }
								},[],VertexShader, FragmentShader);
							}else{
								myMaterial = new Material({
									'uTextureSample': { type: '1i', value: textureSample },
									'uKd': { type: '3fv', value: mat.color.toArray() },
									'uks':{type:'3fv', value: light.entity.mat.color },
									'uLightIntensity':{type:'1f', value: 10.0 }
								},[],VertexShader, FragmentShader);
							}
							
							let meshRender = new MeshRender(renderer.gl, mesh, myMaterial);
							renderer.addMesh(meshRender);
						}
					});
				}, onProgress, onError);
		});
}
