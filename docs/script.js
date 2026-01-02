let Module;

// Load WASM module
MiniqueModule().then(function(mod) {
    Module = mod;
    console.log('Minique WASM loaded successfully!');
    document.getElementById('output').textContent = 'Ready! Click "Run" to execute code.';
});

function compileCode() {
    const code = document.getElementById('editor').value;
    const output = document.getElementById('output');
    
    if (!Module) {
        output.textContent = 'Compiler still loading... Please wait.';
        return;
    }
    
    try {
        // Allocate string in WASM memory
        const codePtr = Module.allocateUTF8(code);
        
        // Call WASM function
        const resultPtr = Module._compile_minique(codePtr);
        
        // Convert result back to JavaScript string
        const result = Module.UTF8ToString(resultPtr);
        
        // Display result
        output.textContent = result || '(no output)';
        
        // Free memory
        Module._free(codePtr);
    } catch (error) {
        output.textContent = `Error: ${error.message}`;
        console.error(error);
    }
}

function clearCode() {
    document.getElementById('editor').value = '';
    document.getElementById('output').textContent = 'Code cleared.';
}

function loadExample() {
    document.getElementById('editor').value = `set a = 10;
set b = 20;
set sum = a + b;
show "Result:";
show sum;

set x = 5;
set y = 3;
set product = x * y;
show "Product:";
show product;`;
    document.getElementById('output').textContent = 'Example loaded. Click "Run" to execute.';
}

// Keyboard shortcut: Ctrl+Enter to run
document.addEventListener('keydown', (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
        e.preventDefault();
        compileCode();
    }
});
