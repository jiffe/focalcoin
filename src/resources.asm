asm(".section .rodata\n.global resource0\nresource0:\n.incbin \"resources/index.tmpl\"");
asm(".section .rodata\n.global resource_size0\nresource_size0:\n.int resource_size0 - resource0");
asm(".section .rodata\n.global resource_name0\nresource_name0:\n.string \"index.tmpl\"");

asm(".section .rodata\n.global resource1\nresource1:\n.incbin \"resources/javascript.js\"");
asm(".section .rodata\n.global resource_size1\nresource_size1:\n.int resource_size1 - resource1");
asm(".section .rodata\n.global resource_name1\nresource_name1:\n.string \"javascript.js\"");

asm(".section .rodata\n.global resource2\nresource2:\n.incbin \"resources/stylesheets.css\"");
asm(".section .rodata\n.global resource_size2\nresource_size2:\n.int resource_size2 - resource2");
asm(".section .rodata\n.global resource_name2\nresource_name2:\n.string \"stylesheets.css\"");

int numResources = 3;