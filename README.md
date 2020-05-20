# btbflush-module

Kernel module to export BTB-flush to userspace. Creates a device `/dev/cool` and allows to send a BTBF command to flush the BTB via ibpb.

## Build

``` sh
make -C module
```

## Insert kernel module

```sh
sudo insmod module/cool.ko
```

## Testing

(Not implemented!)

```sh
make
make test
```

Shows timing difference with and without ibpb.
