let Module;
let isLoading = true;

// Show loading state
function setLoading(loading) {
    isLoading = loading;
    const runBtn = document.getElementById('runBtn');
    const output = document.getElementById('output');
    
    if (loading) {
        runBtn.disabled = true;
        runBtn.textContent = '⏳ Loading...';
        output.textContent = 'Compiler loading... Please wait';
    } else {
        runBtn.disabled = false;
        runBtn.innerHTML = '▶ Run';
        output.textContent = 'Ready! Press Ctrl+Enter or click Run to execute code.';
    }
}

// Initialize loading state
setLoading(true);

// Load WASM module with better error handling
MiniqueModule({
    locateFile: function(path) {
        if (path.endsWith('.wasm')) {
            return 'wasm/minique.wasm';
        }
        return path;
    }
}).then(function(mod) {
    Module = mod;
    console.log('Minique WASM loaded successfully!');
    setLoading(false);
}).catch(function(err) {
    console.error('Failed to load WASM:', err);
    document.getElementById('output').textContent = 
        'Error loading compiler. Please refresh the page.\n\n' + err.message;
});

function compileCode() {
    const code = document.getElementById('editor').value;
    const output = document.getElementById('output');
    
    if (isLoading || !Module) {
        output.textContent = 'Compiler still loading... Please wait a moment.';
        return;
    }
    
    if (!code.trim()) {
        output.textContent = 'Please write some code first!';
        return;
    }
    
    try {
        output.textContent = 'Compiling...';
        
        // Small delay to show "Compiling..." message
        setTimeout(() => {
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
                output.textContent = `Execution Error:\n${error.message}`;
                console.error(error);
            }
        }, 100);
        
    } catch (error) {
        output.textContent = `Error:\n${error.message}`;
        console.error(error);
    }
}

function clearCode() {
    document.getElementById('editor').value = '';
    document.getElementById('output').textContent = 'Code cleared. Write some code and click Run!';
}

function loadExample() {
    document.getElementById('editor').value = `set a = 10;
set b = 20;
set sum = a + b;
show "Sum:";
show sum;

set x = 5;
set y = 3;
set product = x * y;
show "Product:";
show product;

set result = a + b * x;
show "Complex:";
show result;`;
    document.getElementById('output').textContent = 'Example loaded! Click Run to execute.';
}

// Keyboard shortcut: Ctrl+Enter to run
document.addEventListener('keydown', (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
        e.preventDefault();
        if (!isLoading) {
            compileCode();
        }
    }
});

// Auto-load example on first visit
if (localStorage.getItem('minique_visited') !== 'true') {
    loadExample();
    localStorage.setItem('minique_visited', 'true');
}
