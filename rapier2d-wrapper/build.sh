echo "*** Building debug lib..."
cargo build
echo "*** Building release lib..."
cargo build --release
echo "*** Building header..."
cbindgen --config cbindgen.toml --crate rapier2d-wrapper --output includes/rapier2d_wrapper.h
echo "*** Done ..."