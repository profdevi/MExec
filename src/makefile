#v1.0 copyright Comine.com 20200602T1347
#

########################################
all:
	@echo " usage make [check|build|cleanall|wipe]"
	@echo ""
	@echo "    Builds Project into MBuild and installs into MInstall directory"
	@echo ""


########################################
check:
	@cmake --version
	@echo "Check ok.  Continue with make build"	



########################################
build:
	@-rm -rf MBuild
	@-rm -rf MInstall
	@mkdir MBuild
	@mkdir MInstall
	(cd MBuild;cmake -DCMAKE_INSTALL_PREFIX="../MInstall" ..)
	(cd MBuild;cmake --build . --target install --config Release )
	@echo "Built and installed files into MInstall"


####################################
wipe:
	-rm -f MUnitTest.*
	-rm -f MTrace.* 
	-rm -f MMemTrack.*
	-rm -f BaseClass.*
	-rm -f codecheck
	-codewipe -all -w *.h *.cpp *.c
	-HeadIns License.txt *.h *.cpp *.c
	-mclean -f *.h *.cpp *.c
	touch wipe
	@mbanner "Wipped files of debugging code.."



########################################
cleanall:
	@-rm -rf MBuild
	@-rm -rf MInstall
	@echo "Cleaned out directories"	


