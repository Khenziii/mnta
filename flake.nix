
{
  description = "A minimalistic note-taking GTK app";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
        shared = import ./shared.nix { inherit pkgs; };
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "mnta";
          version = "1.0.0";

          src = ./src;

          inherit (shared) buildInputs nativeBuildInputs;

          buildPhase = ''
            make
          '';

          installPhase = ''
            mkdir -p $out/bin
            mv mnta $out/bin
          '';
        };
      }
    );
}

