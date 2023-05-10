.PHONY:all
all:
	@cd CodeHandleServer; make;\
	cd -;\
	cd MainServer; make;\
	cd -;

.PHONY:clean
clean:
	@cd CodeHandleServer; make clean;\
	cd -;\
	cd MainServer; make clean;\
	cd -;\
	rm -rf release;

.PHONY:release
release:
	@mkdir -p release/CodeHandleServer;\
	mkdir -p release/MainServer;\
	mkdir -p release/makeuse;\
	cp -rf CodeHandleServer/CodeHandleServer release/CodeHandleServer/;\
	cp -rf CodeHandleServer/TempFile  release/CodeHandleServer/;\
	cp -rf MainServer/MainServer release/MainServer/;\
	cp -rf MainServer/conf release/MainServer/;\
	cp -rf MainServer/wwwroot release/MainServer/;\
	cp -rf ../makeuse/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64 release/makeuse;\
	cp -rf ../makeuse/ctemplate release/makeuse;


