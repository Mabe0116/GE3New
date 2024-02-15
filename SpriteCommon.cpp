#include "SpriteCommon.h"

#include <wrl.h>
#include <cassert>
#include <dxcapi.h>


#pragma comment (lib,"dxcompiler.lib")

using namespace Microsoft::WRL;

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
    HRESULT result{};
    dxCommon_ = dxCommon;

    //DXC初期化
    ComPtr<IDxcUtils> dxcUtils;
    ComPtr<IDxcCompiler3> dxcCompiler;
    result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
    assert(SUCCEEDED(result));
    result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
    assert(SUCCEEDED(result));

    ComPtr<IDxcIncludeHandler> includeHandler;
    result = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
    assert(SUCCEEDED(result));

    //RootSignature
    D3D12_ROOT_SIGNATURE_DESC descripterRootSignature{};
    descripterRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    //画像用のディスクリプタ作成
    D3D12_DESCRIPTOR_RANGE descriptorRange[1]{};
    descriptorRange[0].BaseShaderRegister = 0;
    descriptorRange[0].NumDescriptors = 1;
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //RootParameter
    D3D12_ROOT_PARAMETER rootParameter[3]{};
    //色
    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[0].Descriptor.ShaderRegister = 0;

    //行列
    rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParameter[1].Descriptor.ShaderRegister = 0;
    //画像
    rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameter[2].DescriptorTable.pDescriptorRanges = descriptorRange;
    rootParameter[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);


    descripterRootSignature.pParameters = rootParameter;
    descripterRootSignature.NumParameters = _countof(rootParameter);

    //Sample設定
    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
    staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
    staticSamplers[0].ShaderRegister = 0;
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    descripterRootSignature.pStaticSamplers = staticSamplers;
    descripterRootSignature.NumStaticSamplers = _countof(staticSamplers);

    //シリアライズとしてパイナリする
    ComPtr<ID3D10Blob> signatureBlob;
    ComPtr<ID3D10Blob> errorBlob;
    result = D3D12SerializeRootSignature(&descripterRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(result)){
        assert(false);
    }

    //パイナリをもとに作成
    result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));

    //InputLayout
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[2] = {};
    inputElementDesc[0].SemanticName = "POSITION";
    inputElementDesc[0].SemanticIndex = 0;
    inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDesc[1].SemanticName = "TEXCOORD";
    inputElementDesc[1].SemanticIndex = 0;
    inputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDesc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    D3D12_INPUT_LAYOUT_DESC inputLayDesc{};
    inputLayDesc.pInputElementDescs = inputElementDesc;
    inputLayDesc.NumElements = _countof(inputElementDesc);

    //BlendStateの設定
    D3D12_BLEND_DESC blendDesc{};
    //すべての色要素を書き込む
    blendDesc.RenderTarget[0].RenderTargetWriteMask =
        D3D12_COLOR_WRITE_ENABLE_ALL;

    //Rasterizer
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    //裏面を描画しない
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    //塗りつぶし
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

    //読み込み処理
    ComPtr<IDxcBlob> vertexShaderBlob = CompilerShader(L"Resources/shaders/SpriteVS.hlsl",
        L"vs_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
    assert(SUCCEEDED(result));

    ComPtr<IDxcBlob> pixelShaderBlob = CompilerShader(L"Resources/shaders/SpritePS.hlsl",
        L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
    assert(SUCCEEDED(result));

    //PipelineState
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayDesc;
    graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));

	/*PipelineStateInitialize();*/
}

void SpriteCommon::PreDraw()
{

}

IDxcBlob* SpriteCommon::CompilerShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
    HRESULT result{};

    //hlsl
    IDxcBlobEncoding* shaderSource = nullptr;
    result = dxcUtils->LoadFile(filePath.c_str(),nullptr, &shaderSource);
    assert(SUCCEEDED(result));

    //読み込んだファイルの内容を設定
    DxcBuffer shaderSourceBuffer;
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8;

    LPCWSTR arguments[] = {
        filePath.c_str(),
        L"-E", L"main",
        L"-T", profile,
        L"-Zi",L"-Qembed_debug",
        L"-Od",
        L"-Zpr"
    };
    IDxcResult* shaderResult = nullptr;
    result = dxcCompiler->Compile(
        &shaderSourceBuffer,
        arguments,
        _countof(arguments),
        includeHandler,
        IID_PPV_ARGS(&shaderResult)
    );
    assert(SUCCEEDED(result));

    IDxcBlobUtf8* shaderError = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
        assert(false);
    }

    IDxcBlob* shaderBlob = nullptr;
    result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
    assert(SUCCEEDED(result));

    shaderSource->Release();
    shaderSource->Release();

    return shaderBlob;
}
