{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        cc = pkgs.gcc12;
      in
      rec
      {
        devShells.default = pkgs.mkShell {
          packages = packages.unit_tests.buildInputs ++ (with pkgs; [
            gcovr
            ltrace
            valgrind
            python311Packages.compiledb
            gdb
            filezilla
            busybox
            inetutils
          ]);
        };

        formatter = pkgs.nixpkgs-fmt;

        packages = {
          default = pkgs.stdenv.mkDerivation rec {
            name = "myftp";
            src = ./.;

            buildInputs = [ cc ] ++ (with pkgs; [ glibc gnumake ]);
            makeFlags = [ "CC=${cc}/bin/gcc" ];
            hardeningDisable = [ "format" "fortify" ];
            enableParallelBuilding = true;

            buildPhase = ''
              make ${name}
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp ${name} $out/bin
            '';
          };

          unit_tests = packages.default.overrideAttrs (final: previous: rec {
            name = "unit_tests";

            buildInputs = previous.buildInputs ++ (with pkgs; [ criterion ]);

            buildPhase = ''
              make ${name}
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp ${name} $out/bin
            '';
          });
        };
      });
}
