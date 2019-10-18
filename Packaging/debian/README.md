First at all you need **./debian** directory in right place or make symlink: `ln -s Packaging/debian .`

Is good idea to change version of created debian package to reflect applied changes (new official current release version ) or patches using `dch` command. 

Then use **debuild** command from package  `devscripts`: `debuild -us -uc`. (The newer command /usr/bin/**debuild-pbuilder** from package **pbuilder**: also works well.)

you have to finally get :

> dpkg-deb: creating package „devilutionx“ in „../devilutionx_0.5.0_amd64.deb“.

It build packages in .. of repository root.  (from ../../debian) 


